#pragma once
#ifdef CAE_ENABLE_GUI

#include "core/ActionManager.h"

// 前向声明避免循环依赖
class MainWindow;

/// @brief UI插件扩展接口
/// UI_ONLY和HYBRID类型的插件实现此接口以贡献UI元素
class IUIPlugin {
public:
    virtual ~IUIPlugin() = default;

    /// @brief 插件向主窗口注入UI元素（面板、停靠窗口等）
    virtual void setupUI(MainWindow* mainWindow) = 0;

    /// @brief 插件移除其UI元素（关闭时调用）
    virtual void teardownUI() = 0;
};

#endif // CAE_ENABLE_GUI
