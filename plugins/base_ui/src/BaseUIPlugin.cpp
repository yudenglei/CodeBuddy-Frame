#include "BaseUIPlugin.h"
#include "core/ActionManager.h"
#include "core/ActionContext.h"
#include <iostream>
#include <cstdlib>

#ifdef CAE_ENABLE_GUI
#include <QApplication>
#endif

// ============================================================================
// 瀵煎嚭鍑芥暟瀹炵幇
// ============================================================================
extern "C" {
    IPlugin* createPlugin()  { return new BaseUIPlugin(); }
    void destroyPlugin(IPlugin* p) { delete p; }
}


// ============================================================================
// IPlugin 瀹炵幇
// ============================================================================
PluginMeta BaseUIPlugin::getMeta() const {
    return PluginMeta{
        "base_ui",
        "1.0.0",
        "Base UI Plugin: ANSYS-style menus and toolbar actions",
        PluginType::UI_ONLY,
        {}  // 鏃犱緷璧?    };
}

bool BaseUIPlugin::isCompatible(RunMode mode) const {
    // UI_ONLY 鎻掍欢鍦ㄦ墍鏈夋ā寮忎笅閮藉彲浠ユ敞鍐孉ction锛堜笉鍚玌I閮ㄥ垎锛?    return true;
}

bool BaseUIPlugin::initialize(RunMode mode) {
    // 娉ㄥ唽ANSYS椋庢牸鐨勮彍鍗?    registerFileActions();
    registerEditActions();
    registerViewActions();
    registerProjectActions();
    registerDrawActions();
    registerModelerActions();
    registerToolsActions();
    
    std::cout << "[BaseUIPlugin] Initialized in mode="
              << (mode == RunMode::GUI ? "GUI" : "non-GUI") << "\n";
    return true;
}

void BaseUIPlugin::shutdown() {
    std::cout << "[BaseUIPlugin] Shutdown\n";
}


// ============================================================================
// Action 娉ㄥ唽锛欶ile 鑿滃崟
// ============================================================================
void BaseUIPlugin::registerFileActions() {
    auto& am = ActionManager::instance();

    // File/New
    ActionDescriptor desc1;
    desc1.id = "File.New";
    desc1.label = "New";
    desc1.tooltip = "Create a new project (Ctrl+N)";
    desc1.shortcut = "Ctrl+N";
    desc1.menuPath = "File";
    desc1.callback = [](const ActionContext&) {
        std::cout << "[Action] File.New executed\n";
    };
    am.registerAction(desc1);

    // File/Open
    ActionDescriptor desc2;
    desc2.id = "File.Open";
    desc2.label = "Open...";
    desc2.tooltip = "Open an existing project (Ctrl+O)";
    desc2.shortcut = "Ctrl+O";
    desc2.menuPath = "File";
    desc2.callback = [](const ActionContext&) {
        std::cout << "[Action] File.Open executed\n";
    };
    am.registerAction(desc2);

    // File/Save
    ActionDescriptor desc3;
    desc3.id = "File.Save";
    desc3.label = "Save";
    desc3.tooltip = "Save current project (Ctrl+S)";
    desc3.shortcut = "Ctrl+S";
    desc3.menuPath = "File";
    desc3.callback = [](const ActionContext&) {
        std::cout << "[Action] File.Save executed\n";
    };
    am.registerAction(desc3);

    // File/Save As
    ActionDescriptor desc4;
    desc4.id = "File.SaveAs";
    desc4.label = "Save As...";
    desc4.tooltip = "Save project to a new location";
    desc4.shortcut = "Ctrl+Shift+S";
    desc4.menuPath = "File";
    desc4.callback = [](const ActionContext&) {
        std::cout << "[Action] File.SaveAs executed\n";
    };
    am.registerAction(desc4);

    // 鍒嗛殧绗?    ActionDescriptor sep1;
    sep1.id = "File.Sep1";
    sep1.label = "";
    sep1.menuPath = "File/---";
    am.registerAction(sep1);

    // File/Exit
    ActionDescriptor desc5;
    desc5.id = "File.Exit";
    desc5.label = "Exit";
    desc5.tooltip = "Exit application";
    desc5.shortcut = "Alt+F4";
    desc5.menuPath = "File";
    desc5.callback = [](const ActionContext&) {
        std::cout << "[Action] File.Exit executed\n";
#ifdef CAE_ENABLE_GUI
        if (QApplication::instance()) {
            QApplication::quit();
        }
#else
        std::exit(0);
#endif
    };
    am.registerAction(desc5);
}


// ============================================================================
// Action 娉ㄥ唽锛欵dit 鑿滃崟
// ============================================================================
void BaseUIPlugin::registerEditActions() {
    auto& am = ActionManager::instance();

    // Edit/Undo
    ActionDescriptor desc1;
    desc1.id = "Edit.Undo";
    desc1.label = "Undo";
    desc1.tooltip = "Undo last operation (Ctrl+Z)";
    desc1.shortcut = "Ctrl+Z";
    desc1.menuPath = "Edit";
    desc1.callback = [](const ActionContext&) {
        std::cout << "[Action] Edit.Undo executed\n";
    };
    am.registerAction(desc1);

    // Edit/Redo
    ActionDescriptor desc2;
    desc2.id = "Edit.Redo";
    desc2.label = "Redo";
    desc2.tooltip = "Redo last operation (Ctrl+Y)";
    desc2.shortcut = "Ctrl+Y";
    desc2.menuPath = "Edit";
    desc2.callback = [](const ActionContext&) {
        std::cout << "[Action] Edit.Redo executed\n";
    };
    am.registerAction(desc2);

    // 鍒嗛殧绗?    ActionDescriptor sep1;
    sep1.id = "Edit.Sep1";
    sep1.label = "";
    sep1.menuPath = "Edit/---";
    am.registerAction(sep1);

    // Edit/Cut
    ActionDescriptor desc3;
    desc3.id = "Edit.Cut";
    desc3.label = "Cut";
    desc3.tooltip = "Cut selection (Ctrl+X)";
    desc3.shortcut = "Ctrl+X";
    desc3.menuPath = "Edit";
    desc3.callback = [](const ActionContext&) {
        std::cout << "[Action] Edit.Cut executed\n";
    };
    am.registerAction(desc3);

    // Edit/Copy
    ActionDescriptor desc4;
    desc4.id = "Edit.Copy";
    desc4.label = "Copy";
    desc4.tooltip = "Copy selection (Ctrl+C)";
    desc4.shortcut= "Ctrl+C";
    desc4.menuPath = "Edit";
    desc4.callback = [](const ActionContext&) {
        std::cout << "[Action] Edit.Copy executed\n";
    };
    am.registerAction(desc4);

    // Edit/Paste
    ActionDescriptor desc5;
    desc5.id = "Edit.Paste";
    desc5.label = "Paste";
    desc5.tooltip = "Paste from clipboard (Ctrl+V)";
    desc5.shortcut = "Ctrl+V";
    desc5.menuPath = "Edit";
    desc5.callback = [](const ActionContext&) {
        std::cout << "[Action] Edit.Paste executed\n";
    };
    am.registerAction(desc5);

    // Edit/Delete
    ActionDescriptor desc6;
    desc6.id = "Edit.Delete";
    desc6.label = "Delete";
    desc6.tooltip = "Delete selection (Del)";
    desc6.shortcut = "Del";
    desc6.menuPath = "Edit";
    desc6.callback = [](const ActionContext&) {
        std::cout << "[Action] Edit.Delete executed\n";
    };
    am.registerAction(desc6);
}


// ============================================================================
// Action 娉ㄥ唽锛歏iew 鑿滃崟
// ============================================================================
void BaseUIPlugin::registerViewActions() {
    auto& am = ActionManager::instance();

    // View/Zoom In
    ActionDescriptor desc1;
    desc1.id = "View.ZoomIn";
    desc1.label = "Zoom In";
    desc1.tooltip = "Zoom in (Ctrl+=)";
    desc1.shortcut = "Ctrl+=";
    desc1.menuPath = "View";
    desc1.callback = [](const ActionContext&) {
        std::cout << "[Action] View.ZoomIn executed\n";
    };
    am.registerAction(desc1);

    // View/Zoom Out
    ActionDescriptor desc2;
    desc2.id = "View.ZoomOut";
    desc2.label = "Zoom Out";
    desc2.tooltip = "Zoom out (Ctrl+-)";
    desc2.shortcut = "Ctrl+-";
    desc2.menuPath = "View";
    desc2.callback = [](const ActionContext&) {
        std::cout << "[Action] View.ZoomOut executed\n";
    };
    am.registerAction(desc2);

    // View/Fit All
    ActionDescriptor desc3;
    desc3.id = "View.FitAll";
    desc3.label = "Fit All";
    desc3.tooltip = "Fit entire design in view (Ctrl+F)";
    desc3.shortcut = "Ctrl+F";
    desc3.menuPath = "View";
    desc3.callback = [](const ActionContext&) {
        std::cout << "[Action] View.FitAll executed\n";
    };
    am.registerAction(desc3);

    // 鍒嗛殧绗?    ActionDescriptor sep1;
    sep1.id = "View.Sep1";
    sep1.label = "";
    sep1.menuPath = "View/---";
    am.registerAction(sep1);

    // View/Pan
    ActionDescriptor desc4;
    desc4.id = "View.Pan";
    desc4.label = "Pan";
    desc4.tooltip = "Pan view (Middle Mouse)";
    desc4.menuPath = "View";
    desc4.callback = [](const ActionContext&) {
        std::cout << "[Action] View.Pan executed\n";
    };
    am.registerAction(desc4);

    // View/Rotate
    ActionDescriptor desc5;
    desc5.id = "View.Rotate";
    desc5.label = "Rotate";
    desc5.tooltip = "Rotate view (Ctrl+Drag)";
    desc5.menuPath = "View";
    desc5.callback = [](const ActionContext&) {
        std::cout << "[Action] View.Rotate executed\n";
    };
    am.registerAction(desc5);
}


// ============================================================================
// Action 娉ㄥ唽锛歅roject 鑿滃崟
// ============================================================================
void BaseUIPlugin::registerProjectActions() {
    auto& am = ActionManager::instance();

    // Project/Insert
    ActionDescriptor desc1;
    desc1.id = "Project.Insert";
    desc1.label = "Insert";
    desc1.tooltip = "Insert existing design";
    desc1.menuPath = "Project";
    desc1.callback = [](const ActionContext&) {
        std::cout << "[Action] Project.Insert executed\n";
    };
    am.registerAction(desc1);

    // Project/Close
    ActionDescriptor desc2;
    desc2.id = "Project.Close";
    desc2.label = "Close";
    desc2.tooltip = "Close current project";
    desc2.menuPath = "Project";
    desc2.callback = [](const ActionContext&) {
        std::cout << "[Action] Project.Close executed\n";
    };
    am.registerAction(desc2);
}


// ============================================================================
// Action 娉ㄥ唽锛欴raw 鑿滃崟锛堝甫Ribbon閰嶇疆锛?// ============================================================================
void BaseUIPlugin::registerDrawActions() {
    auto& am = ActionManager::instance();

    // Draw/Box
    ActionDescriptor desc1;
    desc1.id = "Draw.Box";
    desc1.label = "Box";
    desc1.tooltip = "Create rectangular box";
    desc1.shortcut = "B";
    desc1.menuPath = "Draw/Primitives";
    desc1.ribbon.tabId = "Draw";
    desc1.ribbon.tabTitle = "缁樺埗";
    desc1.ribbon.groupId = "Primitives";
    desc1.ribbon.groupTitle = "鍩烘湰浣?;
    desc1.ribbon.size = ToolButtonSize::Large;
    desc1.ribbon.tabOrder = 10;
    desc1.ribbon.groupOrder = 10;
    desc1.callback = [](const ActionContext&) {
        std::cout << "[Action] Draw.Box executed\n";
    };
    am.registerAction(desc1);

    // Draw/Cylinder
    ActionDescriptor desc2;
    desc2.id = "Draw.Cylinder";
    desc2.label = "Cylinder";
    desc2.tooltip = "Create cylinder";
    desc2.menuPath = "Draw/Primitives";
    desc2.ribbon.tabId = "Draw";
    desc2.ribbon.tabTitle = "缁樺埗";
    desc2.ribbon.groupId = "Primitives";
    desc2.ribbon.groupTitle = "鍩烘湰浣?;
    desc2.ribbon.size = ToolButtonSize::Large;
    desc2.ribbon.tabOrder = 10;
    desc2.ribbon.groupOrder = 10;
    desc2.callback = [](const ActionContext&) {
        std::cout << "[Action] Draw.Cylinder executed\n";
    };
    am.registerAction(desc2);

    // Draw/Sphere
    ActionDescriptor desc3;
    desc3.id = "Draw.Sphere";
    desc3.label = "Sphere";
    desc3.tooltip = "Create sphere";
    desc3.menuPath = "Draw/Primitives";
    desc3.ribbon.tabId = "Draw";
    desc3.ribbon.tabTitle = "缁樺埗";
    desc3.ribbon.groupId = "Primitives";
    desc3.ribbon.groupTitle = "鍩烘湰浣?;
    desc3.ribbon.size = ToolButtonSize::Large;
    desc3.ribbon.tabOrder = 10;
    desc3.ribbon.groupOrder = 10;
    desc3.callback = [](const ActionContext&) {
        std::cout << "[Action] Draw.Sphere executed\n";
    };
    am.registerAction(desc3);

    // Draw/Polygon
    ActionDescriptor desc4;
    desc4.id = "Draw.Polygon";
    desc4.label = "Polygon";
    desc4.tooltip = "Create polygon";
    desc4.menuPath = "Draw/2D Objects";
    desc4.ribbon.tabId = "Draw";
    desc4.ribbon.tabTitle = "缁樺埗";
    desc4.ribbon.groupId = "2D";
    desc4.ribbon.groupTitle = "2D鍥惧舰";
    desc4.ribbon.size = ToolButtonSize::Large;
    desc4.ribbon.tabOrder = 10;
    desc4.ribbon.groupOrder = 20;
    desc4.callback = [](const ActionContext&) {
        std::cout << "[Action] Draw.Polygon executed\n";
    };
    am.registerAction(desc4);
}


// ============================================================================
// Action 娉ㄥ唽锛歁odeler 鑿滃崟锛堝甫Ribbon閰嶇疆锛?// ============================================================================
void BaseUIPlugin::registerModelerActions() {
    auto& am = ActionManager::instance();

    // Modeler/Unite
    ActionDescriptor desc1;
    desc1.id = "Modeler.Unite";
    desc1.label = "Unite";
    desc1.tooltip = "Boolean unite (combine objects)";
    desc1.shortcut = "U";
    desc1.menuPath = "Modeler/Boolean";
    desc1.ribbon.tabId = "Modeler";
    desc1.ribbon.tabTitle = "寤烘ā";
    desc1.ribbon.groupId = "Boolean";
    desc1.ribbon.groupTitle = "甯冨皵杩愮畻";
    desc1.ribbon.size = ToolButtonSize::Large;
    desc1.ribbon.tabOrder = 20;
    desc1.ribbon.groupOrder = 0;
    desc1.callback = [](const ActionContext&) {
        std::cout << "[Action] Modeler.Unite executed\n";
    };
    am.registerAction(desc1);

    // Modeler/Subtract
    ActionDescriptor desc2;
    desc2.id = "Modeler.Subtract";
    desc2.label = "Subtract";
    desc2.tooltip = "Boolean subtract";
    desc2.shortcut = "S";
    desc2.menuPath = "Modeler/Boolean";
    desc2.ribbon.tabId = "Modeler";
    desc2.ribbon.tabTitle = "寤烘ā";
    desc2.ribbon.groupId = "Boolean";
    desc2.ribbon.groupTitle = "甯冨皵杩愮畻";
    desc2.ribbon.size = ToolButtonSize::Large;
    desc2.ribbon.tabOrder = 20;
    desc2.ribbon.groupOrder = 0;
    desc2.callback = [](const ActionContext&) {
        std::cout << "[Action] Modeler.Subtract executed\n";
    };
    am.registerAction(desc2);

    // Modeler/Fillet
    ActionDescriptor desc3;
    desc3.id = "Modeler.Fillet";
    desc3.label = "Fillet";
    desc3.tooltip = "Apply fillet to edges";
    desc3.menuPath = "Modeler/Edge";
    desc3.ribbon.tabId = "Modeler";
    desc3.ribbon.tabTitle = "寤烘ā";
    desc3.ribbon.groupId = "Edge";
    desc3.ribbon.groupTitle = "杈圭紭";
    desc3.ribbon.size = ToolButtonSize::Large;
    desc3.ribbon.tabOrder = 20;
    desc3.ribbon.groupOrder = 10;
    desc3.callback = [](const ActionContext&) {
        std::cout << "[Action] Modeler.Fillet executed\n";
    };
    am.registerAction(desc3);

    // Modeler/Chamfer
    ActionDescriptor desc4;
    desc4.id = "Modeler.Chamfer";
    desc4.label = "Chamfer";
    desc4.tooltip = "Apply chamfer to edges";
    desc4.menuPath = "Modeler/Edge";
    desc4.ribbon.tabId = "Modeler";
    desc4.ribbon.tabTitle = "寤烘ā";
    desc4.ribbon.groupId = "Edge";
    desc4.ribbon.groupTitle = "杈圭紭";
    desc4.ribbon.size = ToolButtonSize::Large;
    desc4.ribbon.tabOrder = 20;
    desc4.ribbon.groupOrder = 10;
    desc4.callback = [](const ActionContext&) {
        std::cout << "[Action] Modeler.Chamfer executed\n";
    };
    am.registerAction(desc4);
}


// ============================================================================
// Action 娉ㄥ唽锛歍ools 鑿滃崟
// ============================================================================
void BaseUIPlugin::registerToolsActions() {
    auto& am = ActionManager::instance();

    // Tools/Options
    ActionDescriptor desc1;
    desc1.id = "Tools.Options";
    desc1.label = "Options...";
    desc1.tooltip = "Application options";
    desc1.menuPath = "Tools";
    desc1.callback = [](const ActionContext&) {
        std::cout << "[Action] Tools.Options executed\n";
    };
    am.registerAction(desc1);

    // Tools/Plugin Manager
    ActionDescriptor desc2;
    desc2.id = "Tools.PluginManager";
    desc2.label = "Plugin Manager...";
    desc2.tooltip = "Manage plugins";
    desc2.menuPath = "Tools";
    desc2.callback = [](const ActionContext&) {
        std::cout << "[Action] Tools.PluginManager executed\n";
    };
    am.registerAction(desc2);
}


// ============================================================================
// IUIPlugin 瀹炵幇锛堜粎 GUI 妯″紡锛?// ============================================================================
#ifdef CAE_ENABLE_GUI
void BaseUIPlugin::setupUI(MainWindow* mainWindow) {
    mainWindow_ = mainWindow;
    std::cout << "[BaseUIPlugin] UI setup complete\n";
}


void BaseUIPlugin::teardownUI() {
    mainWindow_ = nullptr;
}
#endif
