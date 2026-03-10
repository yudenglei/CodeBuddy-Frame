#include "core/IPluginManager.h"
#include "core/IPlugin.h"
#include "core/PluginMeta.h"
#include "core/RunMode.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <filesystem>

// DynamicLoader鍓嶅悜澹版槑锛堝疄鐜板湪DynamicLoader.cpp锛?struct LibHandle;
LibHandle* dynamicLoad(const std::string& path);
void* dynamicSymbol(LibHandle* handle, const std::string& symbol);
void dynamicUnload(LibHandle* handle);
std::vector<std::string> scanDirectory(const std::string& dir, const std::string& ext);

/// @brief 鎻掍欢璁板綍锛堝寘鍚姩鎬佸簱鍙ユ焺鍜屽疄渚嬶級
struct PluginEntry {
    std::string path;
    LibHandle*  libHandle{nullptr};
    IPlugin*    plugin{nullptr};
    DestroyPluginFunc destroyFunc{nullptr};
};

// 鍓嶅悜澹版槑 - 鍦℅UI妯″紡涓嬪寘鍚獻UIPlugin
#ifdef CAE_ENABLE_GUI
class IUIPlugin;
#endif

/// @brief PluginManager瀹炵幇锛圞ahn鎷撴墤鎺掑簭 + 鑷姩鍙戠幇 + UI闆嗘垚锛?class PluginManager : public IPluginManager {
public:
    PluginManager() : pluginDir_("") {}
    ~PluginManager() override { shutdownAll(); }

    /// @brief 璁剧疆鎻掍欢鐩綍璺緞
    void setPluginPath(const std::string& path) {
        pluginDir_ = path;
    }

    bool discover(const std::string& pluginDir) override {
        // 淇濆瓨鎻掍欢鐩綍
        if (!pluginDir.empty()) {
            pluginDir_ = pluginDir;
        }

        // 濡傛灉娌℃湁璁剧疆鎻掍欢鐩綍锛屼娇鐢ㄩ粯璁よ矾寰?        if (pluginDir_.empty()) {
            std::cerr << "[PluginManager] Plugin directory not set!\n";
            return false;
        }

        std::cout << "[PluginManager] Discovering plugins in: " << pluginDir_ << "\n";

        // 鑾峰彇骞冲彴鎵╁睍鍚?#ifdef _WIN32
        const std::string ext = ".dll";
#else
        const std::string ext = ".so";
#endif

        auto files = scanDirectory(pluginDir_, ext);

        if (files.empty()) {
            std::cout << "[PluginManager] No plugins found in: " << pluginDir_ << "\n";
            return false;
        }

        std::cout << "[PluginManager] Found " << files.size() << " plugin file(s)\n";

        for (const auto& file : files) {
            auto* handle = dynamicLoad(file);
            if (!handle) {
                std::cerr << "[PluginManager] Failed to load: " << file << "\n";
                continue;
            }

            auto createFunc = reinterpret_cast<CreatePluginFunc>(
                dynamicSymbol(handle, "createPlugin"));
            auto destroyFunc = reinterpret_cast<DestroyPluginFunc>(
                dynamicSymbol(handle, "destroyPlugin"));

            if (!createFunc || !destroyFunc) {
                std::cerr << "[PluginManager] Missing createPlugin/destroyPlugin in: " << file << "\n";
                dynamicUnload(handle);
                continue;
            }

            IPlugin* plugin = createFunc();
            if (!plugin) {
                dynamicUnload(handle);
                continue;
            }

            PluginEntry entry;
            entry.path        = file;
            entry.libHandle   = handle;
            entry.plugin      = plugin;
            entry.destroyFunc = destroyFunc;

            std::string name = plugin->getMeta().name;
            entries_[name] = std::move(entry);
            std::cout << "[PluginManager] Discovered plugin: " << name << "\n";
        }

        return !entries_.empty();
    }

    bool initializeAll(RunMode mode) override {
        // Kahn鎷撴墤鎺掑簭
        auto sorted = topoSort();

        for (const auto& name : sorted) {
            auto& entry = entries_[name];
            if (!entry.plugin->isCompatible(mode)) {
                std::cout << "[PluginManager] Skip incompatible plugin: " << name << "\n";
continue;
            }
            if (!entry.plugin->initialize(mode)) {
                std::cerr << "[PluginManager] Plugin initialization failed: " << name << "\n";
                return false;
            }
            loadOrder_.push_back(name);
            std::cout << "[PluginManager] Initialized plugin: " << name << "\n";
        }
        return true;
    }

    /// @brief 璁剧疆涓荤獥鍙ｅ苟璋冪敤UI鎻掍欢鐨剆etupUI
    /// 浣跨敤杩愯鏃剁被鍨嬭瘑鍒?RTTI)鏉ユ娴婭UIPlugin鎺ュ彛
    void setMainWindow(void* mainWindow) override {
#ifdef CAE_ENABLE_GUI
        if (!mainWindow) {
            std::cerr << "[PluginManager] setMainWindow: mainWindow is null!\n";
            return;
        }

        std::cout << "[PluginManager] Setting up UI for plugins\n";

        // 浣跨敤dynamic_cast妫€娴婭UIPlugin鎺ュ彛
        // 娉ㄦ剰锛氶渶瑕佸惎鐢≧TTI锛堥粯璁ゅ惎鐢級
        for (const auto& name : loadOrder_) {
            auto& entry = entries_[name];
            if (!entry.plugin) continue;

            // 灏濊瘯杞崲涓篒UIPlugin
            // 杩欓噷浣跨敤绠€鍗曠殑鎺ュ彛妫€娴嬫柟娉?            // 鍦ㄥ疄闄呬娇鐢ㄦ椂锛屾彃浠朵細瀵煎嚭鐗瑰畾鐨勬帴鍙?            
            // 閫氳繃鎻掍欢绫诲瀷鍒ゆ柇鏄惁涓篣I鎻掍欢
            PluginMeta meta = entry.plugin->getMeta();
            if (meta.type == PluginType::UI_ONLY || meta.type == PluginType::HYBRID) {
                std::cout << "[PluginManager] Setting up UI for: " << name << "\n";
                // UI鎻掍欢鐨剆etupUI璋冪敤闇€瑕侀€氳繃鎺ュ彛杩涜
                // 杩欓噷鐣欎綔鎵╁睍鐐?            }
        }
#else
        std::cout << "[PluginManager] UI plugins not supported in non-GUI mode\n";
#endif
    }

    void shutdownAll() override {
        // 閫嗗簭鍏抽棴
        for (auto it = loadOrder_.rbegin(); it != loadOrder_.rend(); ++it) {
            auto eit = entries_.find(*it);
            if (eit != entries_.end() && eit->second.plugin) {
                eit->second.plugin->shutdown();
                if (eit->second.destroyFunc) {
                    eit->second.destroyFunc(eit->second.plugin);
                }
                eit->second.plugin = nullptr;
                if (eit->second.libHandle) {
                    dynamicUnload(eit->second.libHandle);
                    eit->second.libHandle = nullptr;
                }
            }
        }
        loadOrder_.clear();
        entries_.clear();
    }

    IPlugin* getPlugin(const std::string& name) const override {
        auto it = entries_.find(name);
        return (it != entries_.end()) ? it->second.plugin : nullptr;
    }

    std::vector<IPlugin*> getAllPlugins() const override {
        std::vector<IPlugin*> result;
        for (const auto& name : loadOrder_) {
            auto it = entries_.find(name);
            if (it != entries_.end()) result.push_back(it->second.plugin);
        }
        return result;
    }

    std::string getPluginDir() const override {
        return pluginDir_;
    }

private:
    /// @brief Kahn绠楁硶鎷撴墤鎺掑簭
    /// @throws std::runtime_error 瀛樺湪寰幆渚濊禆鏃?    std::vector<std::string> topoSort() {
        // 鏋勫缓鍏ュ害琛ㄥ拰閭绘帴琛?        std::unordered_map<std::string, int> inDegree;
        std::unordered_map<std::string, std::vector<std::string>> dependents; // A->琚獳渚濊禆鐨勮妭鐐?
        for (auto& [name, entry] : entries_) {
            if (inDegree.find(name) == inDegree.end()) inDegree[name] = 0;
            for (const auto& dep : entry.plugin->getMeta().dependencies) {
                // 妫€鏌ヤ緷璧栨槸鍚﹀瓨鍦?                if (entries_.find(dep) == entries_.end()) {
                    std::cerr << "[PluginManager] Warning: Plugin '" << name 
                              << "' depends on missing plugin: " << dep << "\n";
                    continue;
                }
                inDegree[name]++;
                dependents[dep].push_back(name);
            }
        }

        // BFS闃熷垪锛堝叆搴︿负0鐨勮妭鐐癸級
        std::vector<std::string> queue;
        for (auto& [name, deg] : inDegree) {
            if (deg == 0) queue.push_back(name);
        }

        std::vector<std::string> sorted;
        size_t head = 0;
        while (head < queue.size()) {
            std::string cur = queue[head++];
            sorted.push_back(cur);
            for (const auto& dep : dependents[cur]) {
                if (--inDegree[dep] == 0) {
                    queue.push_back(dep);
                }
            }
        }

        if (sorted.size() != entries_.size()) {
            throw std::runtime_error("[PluginManager] Circular dependency detected!");
        }

        return sorted;
    }

    std::unordered_map<std::string, PluginEntry> entries_;
    std::vector<std::string> loadOrder_;
    std::string pluginDir_;
};

// PluginManager宸ュ巶鍑芥暟
std::unique_ptr<IPluginManager> createPluginManager() {
    return std::make_unique<PluginManager>();
}
