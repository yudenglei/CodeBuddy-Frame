#include "core/IPlugin.h"
#include "core/ActionManager.h"
#include "core/ActionContext.h"
#include <iostream>

#ifdef CAE_ENABLE_GUI
#include "ui/IUIPlugin.h"
#include "ui/MainWindow.h"
#include <QDockWidget>
#include <QLabel>
#endif

#ifdef CAE_ENABLE_GUI
class PCBAnalysisPlugin : public IPlugin, public IUIPlugin {
#else
class PCBAnalysisPlugin : public IPlugin {
#endif
public:
    PluginMeta getMeta() const override {
        return {"pcb_analysis", "1.0.0", "PCB Analysis Plugin (HYBRID)", PluginType::HYBRID, {"pcb_db"}};
    }
    bool isCompatible(RunMode) const override { return true; }
    bool initialize(RunMode mode) override {
        mode_ = mode;
        registerToolsActions();
        std::cout << "[PCBAnalysisPlugin] Initialized\n";
        return true;
    }
    void shutdown() override { std::cout << "[PCBAnalysisPlugin] Shutdown\n"; }
#ifdef CAE_ENABLE_GUI
    void setupUI(MainWindow* mainWindow) override {
        auto* dock = new QDockWidget("Analysis Results", mainWindow);
        auto* label = new QLabel("Analysis results will appear here", dock);
        label->setAlignment(Qt::AlignCenter);
        dock->setWidget(label);
        mainWindow->addDockWidget(Qt::BottomDockWidgetArea, dock);
        resultDock_ = dock;
    }
    void teardownUI() override { resultDock_ = nullptr; }
#endif
private:
    void registerToolsActions() {
        auto& am = ActionManager::instance();
        am.registerAction({"Tools.ScriptEditor",      "Script Editor...",   "Open script editor",     "", "",   "tools_menu", [](const ActionContext&){ std::cout << "[Action] Tools.ScriptEditor\n"; }});
        am.registerAction({"Tools.RunScript",          "Run Script...",      "Execute a script file",  "", "F8", "tools_menu",
            [](const ActionContext& ctx){ std::cout << "[Action] Tools.RunScript: " << ctx.get<std::string>("script_path", "(dialog)") << "\n"; }});
        am.registerAction({"Tools.Sep1", "", "", "", "", "tools_menu/---", nullptr});
        am.registerAction({"Tools.ImpedanceAnalysis",  "Impedance Analysis", "Run impedance analysis", "", "",   "tools_menu",
            [this](const ActionContext&){
                std::cout << "[Action] Tools.ImpedanceAnalysis\n";
#ifdef CAE_ENABLE_GUI
                if (mode_ == RunMode::GUI && resultDock_) {
                    if (auto* l = qobject_cast<QLabel*>(resultDock_->widget())) l->setText("Impedance: PASS");
                }
#endif
            }});
        am.registerAction({"Tools.Sep2", "", "", "", "", "tools_menu/---", nullptr});
        am.registerAction({"Tools.PluginManager", "Plugin Manager...", "Manage plugins",    "", "", "tools_menu", [](const ActionContext&){ std::cout << "[Action] Tools.PluginManager\n"; }});
        am.registerAction({"Tools.Options",       "Options...",        "Application options","", "", "tools_menu", [](const ActionContext&){ std::cout << "[Action] Tools.Options\n"; }});
    }
    RunMode mode_{RunMode::HEADLESS};
#ifdef CAE_ENABLE_GUI
    QDockWidget* resultDock_{nullptr};
#endif
};

extern "C" {
    IPlugin* createPlugin()        { return new PCBAnalysisPlugin(); }
    void destroyPlugin(IPlugin* p) { delete p; }
}
