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

/// @brief PCB鍒嗘瀽鎻掍欢锛圚YBRID锛?
///
/// 渚濊禆鍏崇郴锛歱cb_analysis 鈫?pcb_db锛堥€氳繃 PluginMeta.dependencies 澹版槑锛?
/// - 鎵€鏈夋ā寮忥細娉ㄥ唽 Tools.* Actions
/// - 浠?GUI 妯″紡锛氶澶栨彁渚涘垎鏋愮粨鏋滃仠闈犻潰鏉?
#ifdef CAE_ENABLE_GUI
class PCBAnalysisPlugin : public IPlugin, public IUIPlugin {
#else
class PCBAnalysisPlugin : public IPlugin {
#endif
public:
    PluginMeta getMeta() const override {
        return PluginMeta{
            "pcb_analysis",
            "1.0.0",
            "PCB Analysis Plugin: impedance/SI analysis tools (HYBRID)",
            PluginType::HYBRID,
            {"pcb_db"}  // 渚濊禆 pcb_db 鍏堝姞杞?
        };
    }

    bool isCompatible(RunMode /*mode*/) const override {
        return true; // HYBRID 鍦ㄦ墍鏈夋ā寮忎笅鍧囧彲鐢?
    }

    bool initialize(RunMode mode) override {
        mode_ = mode;
        registerToolsActions();
        std::cout << "[PCBAnalysisPlugin] Initialized\n";
        return true;
    }

    void shutdown() override {
        std::cout << "[PCBAnalysisPlugin] Shutdown\n";
    }

#ifdef CAE_ENABLE_GUI
    void setupUI(MainWindow* mainWindow) override {
        // 鍒涘缓鍒嗘瀽缁撴灉鍋滈潬闈㈡澘
        auto* dock = new QDockWidget("Analysis Results", mainWindow);
        dock->setObjectName("AnalysisResultsDock");
        auto* label = new QLabel("Analysis results will appear here", dock);
        label->setAlignment(Qt::AlignCenter);
        dock->setWidget(label);
        mainWindow->addDockWidget(Qt::BottomDockWidgetArea, dock);
        resultDock_ = dock;
        std::cout << "[PCBAnalysisPlugin] UI setup - dock panel created\n";
    }

    void teardownUI() override {
        resultDock_ = nullptr;
    }
#endif

private:
    void registerToolsActions() {
        auto& am = ActionManager::instance();

        am.registerAction({
            "Tools.ScriptEditor", "Script Editor...",
            "Open built-in script editor", "",
            "", "tools_menu",
            [](const ActionContext& ctx) {
                std::cout << "[Action] Tools.ScriptEditor executed\n";
            }
        });

        am.registerAction({
            "Tools.RunScript", "Run Script...",
            "Execute a Python/macro script file", "",
            "F8", "tools_menu",
            [](const ActionContext& ctx) {
                std::string scriptPath = ctx.get<std::string>("script_path", "");
                std::cout << "[Action] Tools.RunScript: "
                          << (scriptPath.empty() ? "(dialog)" : scriptPath) << "\n";
            }
        });

        am.registerAction({
            "Tools.Sep1", "",
            "", "", "", "tools_menu/---",
            nullptr
        });

        am.registerAction({
            "Tools.ImpedanceAnalysis", "Impedance Analysis",
            "Run impedance analysis on selected nets", "",
            "", "tools_menu",
            [this](const ActionContext& ctx) {
                std::cout << "[Action] Tools.ImpedanceAnalysis - mode="
                          << (int)mode_ << "\n";
                // 鍦?HEADLESS/PYTHON/GRPC 妯″紡涓嬭緭鍑烘枃鏈姤鍛?
                // 鍦?GUI 妯″紡涓嬭繕浼氭洿鏂?resultDock_ 闈㈡澘
#ifdef CAE_ENABLE_GUI
                if (mode_ == RunMode::GUI && resultDock_) {
                    auto* label = qobject_cast<QLabel*>(resultDock_->widget());
                    if (label) label->setText("Impedance Analysis: PASS");
                }
#endif
            }
        });

        am.registerAction({
            "Tools.Sep2", "",
            "", "", "", "tools_menu/---",
            nullptr
        });

        am.registerAction({
            "Tools.PluginManager", "Plugin Manager...",
            "View and manage loaded plugins", "",
            "", "tools_menu",
            [](const ActionContext& ctx) {
                std::cout << "[Action] Tools.PluginManager executed\n";
            }
        });

        am.registerAction({
            "Tools.Options", "Options...",
            "Open application options/preferences", "",
            "", "tools_menu",
            [](const ActionContext& ctx) {
                std::cout << "[Action] Tools.Options executed\n";
            }
        });
    }

    RunMode mode_{RunMode::HEADLESS};
#ifdef CAE_ENABLE_GUI
    QDockWidget* resultDock_{nullptr};
#endif
};

// ============================================================
// 瀵煎嚭鍑芥暟
// ============================================================
extern "C" {
    IPlugin* createPlugin()         { return new PCBAnalysisPlugin(); }
    void destroyPlugin(IPlugin* p)  { delete p; }
}
