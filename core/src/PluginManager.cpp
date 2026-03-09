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

// DynamicLoader前向声明（实现在DynamicLoader.cpp）
struct LibHandle;
LibHandle* dynamicLoad(const std::string& path);
void* dynamicSymbol(LibHandle* handle, const std::string& symbol);
void dynamicUnload(LibHandle* handle);
std::vector<std::string> scanDirectory(const std::string& dir, const std::string& ext);

/// @brief 插件记录（包含动态库句柄和实例）
struct PluginEntry {
    std::string path;
    LibHandle*  libHandle{nullptr};
    IPlugin*    plugin{nullptr};
    DestroyPluginFunc destroyFunc{nullptr};
};

/// @brief PluginManager实现（Kahn拓扑排序 + 自动发现）
class PluginManager : public IPluginManager {
public:
    ~PluginManager() override { shutdownAll(); }

    bool discover(const std::string& pluginDir) override {
        // 获取平台扩展名
#ifdef _WIN32
        const std::string ext = ".dll";
#else
        const std::string ext = ".so";
#endif
        auto files = scanDirectory(pluginDir, ext);

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
            entry.path       = file;
            entry.libHandle  = handle;
            entry.plugin     = plugin;
            entry.destroyFunc = destroyFunc;

            std::string name = plugin->getMeta().name;
            entries_[name] = std::move(entry);
        }

        return !entries_.empty();
    }

    bool initializeAll(RunMode mode) override {
        // Kahn拓扑排序
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
            std::cout << "[PluginManager] Loaded: " << name << "\n";
        }
        return true;
    }

    void shutdownAll() override {
        // 逆序关闭
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

private:
    /// @brief Kahn算法拓扑排序
    /// @throws std::runtime_error 存在循环依赖时
    std::vector<std::string> topoSort() {
        // 构建入度表和邻接表
        std::unordered_map<std::string, int> inDegree;
        std::unordered_map<std::string, std::vector<std::string>> dependents; // A->被A依赖的节点

        for (auto& [name, entry] : entries_) {
            if (inDegree.find(name) == inDegree.end()) inDegree[name] = 0;
            for (const auto& dep : entry.plugin->getMeta().dependencies) {
                inDegree[name]++;
                dependents[dep].push_back(name);
            }
        }

        // BFS队列（入度为0的节点）
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
};

// PluginManager工厂函数
std::unique_ptr<IPluginManager> createPluginManager() {
    return std::make_unique<PluginManager>();
}
