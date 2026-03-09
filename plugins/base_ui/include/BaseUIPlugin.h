#pragma once
#include "core/IPlugin.h"
#include "core/PluginMeta.h"

#ifdef CAE_ENABLE_GUI
#include "ui/IUIPlugin.h"
class MainWindow;
#endif

/// @brief 基础UI插件（UI_ONLY）
///
/// 负责注册 Desktop.* 和 View.* 系列 Actions，
/// 提供项目文件管理和视图控制功能。
/// isCompatible() 仅在 GUI 模式下返回 true。
#ifdef CAE_ENABLE_GUI
class BaseUIPlugin : public IPlugin, public IUIPlugin {
#else
class BaseUIPlugin : public IPlugin {
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
#endif

private:
    void registerDesktopActions();
    void registerViewActions();

#ifdef CAE_ENABLE_GUI
    MainWindow* mainWindow_{nullptr};
#endif
};

// ============================================================
// 导出函数
// ============================================================
extern "C" {
    IPlugin* createPlugin();
    void destroyPlugin(IPlugin* plugin);
}
