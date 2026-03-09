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

/// @brief PCB分析插件（HYBRID）
///
/// 依赖关系：pcb_analysis → pcb_db（通过 PluginMeta.dependencies 声明）
/// - 所有模式：注册 Tools.* Actions
/// - 仅 GUI 模式：额外提供分析结果停靠面板
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
            {"pcb_db"}  // 依赖 pcb_db 先加载
        };
    }

    bool isCompatible(RunMode /*mode*/) const override {
        return true; // HYBRID 在所有模式下均可用
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
        // 创建分析结果停靠面板
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
                // 在 HEADLESS/PYTHON/GRPC 模式下输出文本报告
                // 在 GUI 模式下还会更新 resultDock_ 面板
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
// 导出函数
// ============================================================
extern "C" {
    IPlugin* createPlugin()         { return new PCBAnalysisPlugin(); }
    void destroyPlugin(IPlugin* p)  { delete p; }
}
