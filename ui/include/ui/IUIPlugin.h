#pragma once
#ifdef CAE_ENABLE_GUI

#include "core/PluginGlobal.h"
#include "core/ActionManager.h"

// 前向声明避免循环依赖
class MainWindow;

/// @brief UI插件扩展接口
/// UI_ONLY和HYBRID类型的插件实现此接口以贡献UI元素
/// 
/// 使用方法：
/// 1. 插件在 initialize() 阶段被识别为 IUIPlugin 类型
/// 2. 宿主程序调用 setupUI(mainWindow) 传入主窗口指针
/// 3. 插件在 setupUI 中保存 mainWindow 指针，并创建UI元素
/// 4. 插件在 teardownUI 中清理UI元素
class CAE_PLUGIN_EXPORT IUIPlugin {
public:
    virtual ~IUIPlugin() = default;

    /// @brief 插件向主窗口注入UI元素（面板、停靠窗口等）
    /// @param mainWindow 主窗口指针，插件应保存此指针用于创建对话框等
    virtual void setupUI(MainWindow* mainWindow) = 0;

    /// @brief 插件移除其UI元素（关闭时调用）
    virtual void teardownUI() = 0;
};

#endif // CAE_ENABLE_GUI
