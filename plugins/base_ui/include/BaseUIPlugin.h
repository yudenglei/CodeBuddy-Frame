#pragma once
#include "core/IPlugin.h"
#include "core/PluginMeta.h"

#ifdef CAE_ENABLE_GUI
#include "ui/IUIPlugin.h"
class MainWindow;
#endif

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

extern "C" {
    IPlugin* createPlugin();
    void destroyPlugin(IPlugin* plugin);
}
