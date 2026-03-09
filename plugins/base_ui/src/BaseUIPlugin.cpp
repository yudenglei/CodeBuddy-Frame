#include "BaseUIPlugin.h"
#include "core/ActionManager.h"
#include "core/ActionContext.h"
#include <iostream>

extern "C" {
    IPlugin* createPlugin()  { return new BaseUIPlugin(); }
    void destroyPlugin(IPlugin* p) { delete p; }
}

PluginMeta BaseUIPlugin::getMeta() const {
    return {"base_ui", "1.0.0", "Base UI Plugin: Desktop/View menu actions", PluginType::UI_ONLY, {}};
}

bool BaseUIPlugin::isCompatible(RunMode) const { return true; }

bool BaseUIPlugin::initialize(RunMode mode) {
    registerDesktopActions();
    registerViewActions();
    std::cout << "[BaseUIPlugin] Initialized mode=" << (mode == RunMode::GUI ? "GUI" : "non-GUI") << "\n";
    return true;
}

void BaseUIPlugin::shutdown() { std::cout << "[BaseUIPlugin] Shutdown\n"; }

void BaseUIPlugin::registerDesktopActions() {
    auto& am = ActionManager::instance();
    am.registerAction({"Desktop.NewProject",  "New Project",  "Create a new PCB project",         "", "Ctrl+N",       "desktop_menu", [](const ActionContext&){ std::cout << "[Action] Desktop.NewProject\n"; }});
    am.registerAction({"Desktop.OpenProject", "Open Project...", "Open an existing PCB project", "", "Ctrl+O",       "desktop_menu", [](const ActionContext&){ std::cout << "[Action] Desktop.OpenProject\n"; }});
    am.registerAction({"Desktop.Sep1", "", "", "", "", "desktop_menu/---", nullptr});
    am.registerAction({"Desktop.Save",   "Save",     "Save current project",           "", "Ctrl+S",       "desktop_menu", [](const ActionContext&){ std::cout << "[Action] Desktop.Save\n"; }});
    am.registerAction({"Desktop.SaveAs", "Save As...", "Save project to a new location", "", "Ctrl+Shift+S", "desktop_menu", [](const ActionContext&){ std::cout << "[Action] Desktop.SaveAs\n"; }});
    am.registerAction({"Desktop.Sep2", "", "", "", "", "desktop_menu/---", nullptr});
    am.registerAction({"Desktop.Exit", "Exit", "Exit application", "", "Alt+F4", "desktop_menu",
        [](const ActionContext&){ std::cout << "[Action] Desktop.Exit\n";
#ifdef CAE_ENABLE_GUI
            qApp->quit();
#else
            std::exit(0);
#endif
        }
    });
}

void BaseUIPlugin::registerViewActions() {
    auto& am = ActionManager::instance();
    am.registerAction({"View.ZoomIn",  "Zoom In",  "Zoom into the layout",      "", "Ctrl+=", "view_menu", [](const ActionContext&){ std::cout << "[Action] View.ZoomIn\n"; }});
    am.registerAction({"View.ZoomOut", "Zoom Out", "Zoom out of the layout",    "", "Ctrl+-", "view_menu", [](const ActionContext&){ std::cout << "[Action] View.ZoomOut\n"; }});
    am.registerAction({"View.FitAll",  "Fit All",  "Fit entire layout into view","", "Ctrl+F", "view_menu", [](const ActionContext&){ std::cout << "[Action] View.FitAll\n"; }});
    am.registerAction({"View.Sep1", "", "", "", "", "view_menu/---", nullptr});
    am.registerAction({"View.Pan",  "Pan Mode", "Switch to pan/scroll mode",  "", "P",      "view_menu", [](const ActionContext&){ std::cout << "[Action] View.Pan\n"; }});
    am.registerAction({"View.Sep2", "", "", "", "", "view_menu/---", nullptr});
    am.registerAction({"View.GridSettings",   "Grid Settings...",   "Configure grid settings",   "", "", "view_menu", [](const ActionContext&){ std::cout << "[Action] View.GridSettings\n"; }});
    am.registerAction({"View.DisplayOptions", "Display Options...", "Configure display options", "", "", "view_menu", [](const ActionContext&){ std::cout << "[Action] View.DisplayOptions\n"; }});
}

#ifdef CAE_ENABLE_GUI
void BaseUIPlugin::setupUI(MainWindow* mw) { mainWindow_ = mw; std::cout << "[BaseUIPlugin] UI setup\n"; }
void BaseUIPlugin::teardownUI() { mainWindow_ = nullptr; }
#endif
