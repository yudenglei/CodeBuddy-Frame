#pragma once
#include "IPlugin.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

/// @brief 全局插件注册表（单例模式）
///
/// 功能：
/// - 提供全局插件注册和查询功能
/// - 与PluginManager协同工作
/// - 支持插件依赖解析和生命周期管理
class PluginRegistry {
public:
    /// @brief 获取单例实例
    static PluginRegistry& instance();

    /// @brief 注册插件
    /// @param name 插件名称（唯一标识）
    /// @param plugin 插件实例指针
    /// @return true=注册成功，false=名称已存在
    bool registerPlugin(const std::string& name, IPlugin* plugin);

    /// @brief 注销插件
    /// @param name 插件名称
    /// @return true=注销成功，false=插件不存在
    bool unregisterPlugin(const std::string& name);

    /// @brief 获取插件实例
    /// @param name 插件名称
    /// @return 插件实例指针，不存在时返回nullptr
    IPlugin* getPlugin(const std::string& name) const;

    /// @brief 获取所有插件实例
    /// @return 插件实例指针列表
    std::vector<IPlugin*> getAllPlugins() const;

    /// @brief 获取所有插件名称
    /// @return 插件名称列表
    std::vector<std::string> getPluginNames() const;

    /// @brief 检查插件是否存在
    /// @param name 插件名称
    /// @return true=存在，false=不存在
    bool hasPlugin(const std::string& name) const;

    /// @brief 清空所有插件
    /// @warning 谨慎使用，不会调用插件的shutdown方法
    void clear();

    /// @brief 获取插件数量
    /// @return 已注册插件数量
    size_t count() const { return plugins_.size(); }

    /// @brief 检查注册表是否为空
    /// @return true=空，false=非空
    bool empty() const { return plugins_.empty(); }

private:
    PluginRegistry() = default;
    ~PluginRegistry() = default;

    PluginRegistry(const PluginRegistry&) = delete;
    PluginRegistry& operator=(const PluginRegistry&) = delete;

    std::unordered_map<std::string, IPlugin*> plugins_;
};