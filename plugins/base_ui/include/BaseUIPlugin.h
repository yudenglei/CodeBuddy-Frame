#pragma once
#include "core/IPlugin.h"
#include "core/PluginMeta.h"
#include "core/PluginGlobal.h"

#ifdef CAE_ENABLE_GUI
#include "ui/IUIPlugin.h"
class MainWindow;
#endif

/// @brief 基础UI插件（UI_ONLY）
///
/// 负责注册 ANSYS 风格的菜单和工具栏：
/// - 顶层菜单：File/Edit/View/Project/Draw/Modeler/HFSS/Tools/Window/Help
/// - 工具栏：Draw/Modeler/HFSS 等选项卡的分组按钮
#ifdef CAE_ENABLE_GUI
class CAE_PLUGIN_EXPORT BaseUIPlugin : public IPlugin, public IUIPlugin {
#else
class CAE_PLUGIN_EXPORT BaseUIPlugin : public IPlugin {
#endif
public:
    BaseUIPlugin() = default;
    ~BaseUIPlugin() override = default;

    PluginMeta getMeta() const override;
    bool initialize(RunMode mode) override;
    void shutdown() override;
    bool isCompatible(RunMode mode) const override;

#ifdef CAE_ENABLE_GUI
    void setupUI(MainWindow* mainWindow) override;
    void teardownUI() override;
    MainWindow* getMainWindow() const { return mainWindow_; }
#endif

private:
    // 菜单注册
    void registerFileActions();
    void registerEditActions();
    void registerViewActions();
    void registerProjectActions();
    void registerDrawActions();
    void registerModelerActions();
    void registerToolsActions();

#ifdef CAE_ENABLE_GUI
    MainWindow* mainWindow_{nullptr};
#endif
};

// ============================================================================
// 导出函数（C接口，使用 extern "C" 确保符号一致）
// ============================================================================
extern "C" {
    CAE_PLUGIN_EXPORT IPlugin* createPlugin();
    CAE_PLUGIN_EXPORT void destroyPlugin(IPlugin* plugin);
}
