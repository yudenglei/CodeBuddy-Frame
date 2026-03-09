#pragma once
#include "PluginMeta.h"
#include "RunMode.h"

/// @brief 插件基接口
/// 每个插件动态库必须实现此接口并导出工厂函数
class IPlugin {
public:
    virtual ~IPlugin() = default;

    /// @brief 获取插件元信息（名称/版本/类型/依赖）
    virtual PluginMeta getMeta() const = 0;

    /// @brief 初始化插件（注册Actions，建立DB连接等）
    virtual bool initialize(RunMode mode) = 0;

    /// @brief 关闭插件（释放资源）
    virtual void shutdown() = 0;

    /// @brief 检查插件是否兼容当前运行模式
    virtual bool isCompatible(RunMode mode) const = 0;
};

extern "C" {
    typedef IPlugin* (*CreatePluginFunc)();
    typedef void (*DestroyPluginFunc)(IPlugin*);
}
