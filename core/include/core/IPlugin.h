#pragma once
#include "PluginGlobal.h"
#include "PluginMeta.h"
#include "RunMode.h"

// ============================================================================
// IPlugin.h - 插件基接口
// ============================================================================
// 每个插件动态库必须实现此接口并导出工厂函数
// 
// 导出函数使用 CAE_PLUGIN_EXPORT 宏，确保跨平台编译正确
// 在插件的 CMakeLists.txt 中必须定义插件专属宏，如：-DCAE_PLUGIN_BASE_UI
// ============================================================================

/// @brief 插件基接口
class CAE_PLUGIN_EXPORT IPlugin {
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


// ============================================================================
// 插件工厂函数类型定义
// ============================================================================

/// @brief 创建插件实例（工厂函数）
using CreatePluginFunc = IPlugin* (*)();
/// @brief 销毁插件实例
using DestroyPluginFunc = void (*)(IPlugin*);


// ============================================================================
// 导出函数（C接口，使用 extern "C" 确保符号一致）
// ============================================================================

extern "C" {
    /// @brief 创建插件实例（工厂函数）
    /// @return 插件实例指针
    CAE_PLUGIN_EXPORT IPlugin* createPlugin();
    
    /// @brief 销毁插件实例
    /// @param plugin 要销毁的插件实例指针
    CAE_PLUGIN_EXPORT void destroyPlugin(IPlugin* plugin);
}
