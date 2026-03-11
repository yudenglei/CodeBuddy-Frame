#pragma once
#include <string>
#include <vector>
#include "IPlugin.h"
#include "RunMode.h"
#include "PluginGlobal.h"

/// @brief 插件管理器接口
/// 
/// 负责插件的发现、加载、初始化和卸载
class CAE_PLUGIN_EXPORT IPluginManager {
public:
    virtual ~IPluginManager() = default;

    /// @brief 扫描目录，发现并加载所有插件（含依赖排序）
    /// @param pluginDir 插件目录路径，建议使用绝对路径或相对于可执行文件的路径
    /// @return 是否发现至少一个插件
    virtual bool discover(const std::string& pluginDir) = 0;

    /// @brief 按运行模式初始化所有已发现的兼容插件
    /// @param mode 运行模式（GUI/HEADLESS/PYTHON/GRPC）
    /// @return 是否所有插件初始化成功
    virtual bool initializeAll(RunMode mode) = 0;

    /// @brief 为UI插件设置主窗口（必须在initializeAll之后调用）
    /// @param mainWindow 主窗口指针（仅在GUI模式下需要调用）
    /// 此方法会遍历所有已加载的插件，调用IUIPlugin::setupUI()
    virtual void setMainWindow(void* mainWindow) = 0;

    /// @brief 关闭所有插件（逆序）
    virtual void shutdownAll() = 0;

    /// @brief 按名称获取插件实例
    virtual IPlugin* getPlugin(const std::string& name) const = 0;

    /// @brief 获取所有已加载插件
    virtual std::vector<IPlugin*> getAllPlugins() const = 0;

    /// @brief 获取插件加载目录
    virtual std::string getPluginDir() const = 0;
};
