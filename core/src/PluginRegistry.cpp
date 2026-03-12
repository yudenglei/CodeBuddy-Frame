#include "core/PluginRegistry.h"
#include <algorithm>
#include <iostream>

// ============================================================================
// 单例实例
// ============================================================================

PluginRegistry& PluginRegistry::instance() {
    static PluginRegistry inst;
    return inst;
}

// ============================================================================
// 插件注册管理
// ============================================================================

bool PluginRegistry::registerPlugin(const std::string& name, IPlugin* plugin) {
    if (!plugin) {
        std::cerr << "[PluginRegistry] Cannot register null plugin: " << name << "\n";
        return false;
    }

    if (plugins_.find(name) != plugins_.end()) {
        std::cerr << "[PluginRegistry] Plugin already registered: " << name << "\n";
        return false;
    }

    plugins_[name] = plugin;
    std::cout << "[PluginRegistry] Registered plugin: " << name << "\n";
    return true;
}

bool PluginRegistry::unregisterPlugin(const std::string& name) {
    auto it = plugins_.find(name);
    if (it == plugins_.end()) {
        std::cerr << "[PluginRegistry] Plugin not found: " << name << "\n";
        return false;
    }

    plugins_.erase(it);
    std::cout << "[PluginRegistry] Unregistered plugin: " << name << "\n";
    return true;
}

IPlugin* PluginRegistry::getPlugin(const std::string& name) const {
    auto it = plugins_.find(name);
    if (it == plugins_.end()) {
        return nullptr;
    }
    return it->second;
}

std::vector<IPlugin*> PluginRegistry::getAllPlugins() const {
    std::vector<IPlugin*> result;
    result.reserve(plugins_.size());

    for (const auto& pair : plugins_) {
        result.push_back(pair.second);
    }

    return result;
}

std::vector<std::string> PluginRegistry::getPluginNames() const {
    std::vector<std::string> result;
    result.reserve(plugins_.size());

    for (const auto& pair : plugins_) {
        result.push_back(pair.first);
    }

    // 按名称排序以便一致输出
    std::sort(result.begin(), result.end());
    return result;
}

bool PluginRegistry::hasPlugin(const std::string& name) const {
    return plugins_.find(name) != plugins_.end();
}

void PluginRegistry::clear() {
    std::cout << "[PluginRegistry] Clearing all plugins (count=" << plugins_.size() << ")\n";
    plugins_.clear();
}