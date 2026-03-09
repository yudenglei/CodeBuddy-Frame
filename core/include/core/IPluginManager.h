#pragma once
#include <string>
#include <vector>
#include "IPlugin.h"
#include "RunMode.h"

/// @brief 插件管理器接口
class IPluginManager {
public:
    virtual ~IPluginManager() = default;

    /// @brief 扫描目录，发现并加载所有插件（含依赖排序）
    virtual bool discover(const std::string& pluginDir) = 0;

    /// @brief 按运行模式初始化所有已发现的兼容插件
    virtual bool initializeAll(RunMode mode) = 0;

    /// @brief 关闭所有插件（逆序）
    virtual void shutdownAll() = 0;

    /// @brief 按名称获取插件实例
    virtual IPlugin* getPlugin(const std::string& name) const = 0;

    /// @brief 获取所有已加载插件
    virtual std::vector<IPlugin*> getAllPlugins() const = 0;
};
