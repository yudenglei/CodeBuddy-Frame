#include "BaseUIPlugin.h"
#include "core/ActionManager.h"
#include "core/ActionContext.h"
#include <iostream>

// ============================================================
// 导出函数实现
// ============================================================
extern "C" {
    IPlugin* createPlugin()  { return new BaseUIPlugin(); }
    void destroyPlugin(IPlugin* p) { delete p; }
}

// ============================================================
// IPlugin 实现
// ============================================================
PluginMeta BaseUIPlugin::getMeta() const {
    return PluginMeta{
        "base_ui",
        "1.0.0",
        "Base UI Plugin: Desktop/View menu actions",
        PluginType::UI_ONLY,
        {}  // 无依赖
    };
}

bool BaseUIPlugin::isCompatible(RunMode mode) const {
    // UI_ONLY 插件仅在 GUI 模式下有意义
    // 但 Desktop.Save 等动作在 HEADLESS 模式下也可以运行（不含 UI 部分）
    // 此处演示：所有模式都兼容，非GUI模式跳过UI初始化即可
    return true;
}

bool BaseUIPlugin::initialize(RunMode mode) {
    registerDesktopActions();
    registerViewActions();
    std::cout << "[BaseUIPlugin] Initialized in mode="
              << (mode == RunMode::GUI ? "GUI" : "non-GUI") << "\n";
    return true;
}

void BaseUIPlugin::shutdown() {
    std::cout << "[BaseUIPlugin] Shutdown\n";
}

// ============================================================
// Action 注册：Desktop 菜单
// ============================================================
void BaseUIPlugin::registerDesktopActions() {
    auto& am = ActionManager::instance();

    am.registerAction({
        "Desktop.NewProject", "New Project",
        "Create a new PCB project", "",
        "Ctrl+N", "desktop_menu",
        [](const ActionContext& ctx) {
            std::cout << "[Action] Desktop.NewProject executed\n";
        }
    });

    am.registerAction({
        "Desktop.OpenProject", "Open Project...",
        "Open an existing PCB project", "",
        "Ctrl+O", "desktop_menu",
        [](const ActionContext& ctx) {
            std::cout << "[Action] Desktop.OpenProject executed\n";
        }
    });

    // 分隔符（通过注册一个特殊 separator action 实现）
    am.registerAction({
        "Desktop.Sep1", "",
        "", "", "", "desktop_menu/---",  // menuPath 末段"---"表示分隔符
        nullptr
    });

    am.registerAction({
        "Desktop.Save", "Save",
        "Save current project", "",
        "Ctrl+S", "desktop_menu",
        [](const ActionContext& ctx) {
            std::cout << "[Action] Desktop.Save executed\n";
        }
    });

    am.registerAction({
        "Desktop.SaveAs", "Save As...",
        "Save project to a new location", "",
        "Ctrl+Shift+S", "desktop_menu",
        [](const ActionContext& ctx) {
            std::cout << "[Action] Desktop.SaveAs executed\n";
        }
    });

    am.registerAction({
        "Desktop.Sep2", "",
        "", "", "", "desktop_menu/---",
        nullptr
    });

    am.registerAction({
        "Desktop.Exit", "Exit",
        "Exit application", "",
        "Alt+F4", "desktop_menu",
        [](const ActionContext& ctx) {
            std::cout << "[Action] Desktop.Exit executed\n";
#ifdef CAE_ENABLE_GUI
            qApp->quit();
#else
            std::exit(0);
#endif
        }
    });
}

// ============================================================
// Action 注册：View 菜单
// ============================================================
void BaseUIPlugin::registerViewActions() {
    auto& am = ActionManager::instance();

    am.registerAction({
        "View.ZoomIn", "Zoom In",
        "Zoom into the layout", "",
        "Ctrl+=", "view_menu",
        [](const ActionContext& ctx) {
            std::cout << "[Action] View.ZoomIn executed\n";
        }
    });

    am.registerAction({
        "View.ZoomOut", "Zoom Out",
        "Zoom out of the layout", "",
        "Ctrl+-", "view_menu",
        [](const ActionContext& ctx) {
            std::cout << "[Action] View.ZoomOut executed\n";
        }
    });

    am.registerAction({
        "View.FitAll", "Fit All",
        "Fit entire layout into view", "",
        "Ctrl+F", "view_menu",
        [](const ActionContext& ctx) {
            std::cout << "[Action] View.FitAll executed\n";
        }
    });

    am.registerAction({
        "View.Sep1", "",
        "", "", "", "view_menu/---",
        nullptr
    });

    am.registerAction({
        "View.Pan", "Pan Mode",
        "Switch to pan/scroll mode", "",
        "P", "view_menu",
        [](const ActionContext& ctx) {
            std::cout << "[Action] View.Pan executed\n";
        }
    });

    am.registerAction({
        "View.Sep2", "",
        "", "", "", "view_menu/---",
        nullptr
    });

    am.registerAction({
        "View.GridSettings", "Grid Settings...",
        "Configure grid display settings", "",
        "", "view_menu",
        [](const ActionContext& ctx) {
            std::cout << "[Action] View.GridSettings executed\n";
        }
    });

    am.registerAction({
        "View.DisplayOptions", "Display Options...",
        "Configure layer and display options", "",
        "", "view_menu",
        [](const ActionContext& ctx) {
            std::cout << "[Action] View.DisplayOptions executed\n";
        }
    });
}

// ============================================================
// IUIPlugin 实现（仅 GUI 模式）
// ============================================================
#ifdef CAE_ENABLE_GUI
void BaseUIPlugin::setupUI(MainWindow* mainWindow) {
    mainWindow_ = mainWindow;
    std::cout << "[BaseUIPlugin] UI setup complete\n";
}

void BaseUIPlugin::teardownUI() {
    mainWindow_ = nullptr;
}
#endif
