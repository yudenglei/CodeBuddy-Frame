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
    /// @param mode 当前运行模式，插件据此决定初始化内容
    /// @return true=成功，false=初始化失败（插件管理器将跳过此插件）
    virtual bool initialize(RunMode mode) = 0;

    /// @brief 关闭插件（释放资源）
    virtual void shutdown() = 0;

    /// @brief 检查插件是否兼容当前运行模式
    virtual bool isCompatible(RunMode mode) const = 0;
};

// ============================================================
// 每个插件动态库必须导出以下两个C函数
// ============================================================
extern "C" {
    /// @brief 创建插件实例（工厂函数）
    typedef IPlugin* (*CreatePluginFunc)();
    /// @brief 销毁插件实例
    typedef void (*DestroyPluginFunc)(IPlugin*);
}
