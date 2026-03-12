#pragma once
#ifdef CAE_ENABLE_GUI

#include "core/PluginGlobal.h"
#include "core/ActionManager.h"
#include <string>
#include <vector>

// 前向声明避免循环依赖
class MainWindow;
class QToolBar;
class QMenu;

/// @brief 菜单贡献项
struct MenuContribution {
    std::string id;
    std::string label;
    std::string actionId;  // 关联的Action ID
    std::string menuPath;  // 菜单路径，如 "File/New"
    std::string iconPath;  // 图标路径（可选）
    std::string shortcut;  // 快捷键（可选）
};

/// @brief 工具栏贡献项
struct ToolbarContribution {
    std::string id;
    std::string label;
    std::string actionId;  // 关联的Action ID
    std::string iconPath;  // 图标路径
    std::string tooltip;   // 工具提示
};

/// @brief Ribbon贡献项
struct RibbonContribution {
    std::string tabId;     // Tab ID，如 "Draw"
    std::string tabTitle;  // Tab标题
    std::string groupId;   // Group ID，如 "Primitives"
    std::string groupTitle; // Group标题
    std::string actionId;  // 关联的Action ID
    std::string iconPath;  // 图标路径
    std::string tooltip;   // 工具提示
    int tabOrder{0};       // Tab排序（越小越靠前）
    int groupOrder{0};     // Group排序
    int buttonOrder{0};    // 按钮在Group内排序
};

/// @brief UI插件扩展接口
/// UI_ONLY和HYBRID类型的插件实现此接口以贡献UI元素
///
/// 使用方法：
/// 1. 插件在 initialize() 阶段被识别为 IUIPlugin 类型
/// 2. 宿主程序调用 setupUI(mainWindow) 传入主窗口指针
/// 3. 插件通过 getMenuContributions() 等方法贡献UI元素
/// 4. 宿主程序根据贡献构建UI
/// 5. 插件在 teardownUI 中清理UI元素
class CAE_PLUGIN_EXPORT IUIPlugin {
public:
    virtual ~IUIPlugin() = default;

    /// @brief 插件向主窗口注入UI元素（面板、停靠窗口等）
    /// @param mainWindow 主窗口指针，插件应保存此指针用于创建对话框等
    virtual void setupUI(MainWindow* mainWindow) = 0;

    /// @brief 插件移除其UI元素（关闭时调用）
    virtual void teardownUI() = 0;

    /// @brief 获取菜单贡献列表
    /// @return 菜单贡献项列表
    virtual std::vector<MenuContribution> getMenuContributions() const { return {}; }

    /// @brief 获取工具栏贡献列表
    /// @return 工具栏贡献项列表
    virtual std::vector<ToolbarContribution> getToolbarContributions() const { return {}; }

    /// @brief 获取Ribbon贡献列表
    /// @return Ribbon贡献项列表
    virtual std::vector<RibbonContribution> getRibbonContributions() const { return {}; }

    /// @brief 更新UI状态（响应应用状态变化）
    /// 例如：启用/禁用菜单项，更新工具栏按钮状态等
    virtual void updateUIState() {}

    /// @brief 获取插件贡献的停靠窗口
    /// @return 停靠窗口指针列表
    virtual std::vector<QDockWidget*> getDockWidgets() const { return {}; }

    /// @brief 获取插件贡献的工具窗口
    /// @return 工具窗口指针列表
    virtual std::vector<QWidget*> getToolWindows() const { return {}; }
};

#endif // CAE_ENABLE_GUI
