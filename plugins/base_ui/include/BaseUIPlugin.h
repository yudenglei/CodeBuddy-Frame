#pragma once
#include "core/IPlugin.h"
#include "core/PluginMeta.h"
#include "core/PluginGlobal.h"

#ifdef CAE_ENABLE_GUI
#include "ui/IUIPlugin.h"
class MainWindow;
#endif

/// @brief 鍩虹UI鎻掍欢锛圲I_ONLY锛?///
/// 璐熻矗娉ㄥ唽 ANSYS 椋庢牸鐨勮彍鍗曞拰宸ュ叿鏍忥細
/// - 椤跺眰鑿滃崟锛欶ile/Edit/View/Project/Draw/Modeler/HFSS/Tools/Window/Help
/// - 宸ュ叿鏍忥細Draw/Modeler/HFSS 绛夐€夐」鍗＄殑鍒嗙粍鎸夐挳
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
    // 鑿滃崟娉ㄥ唽
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
// 瀵煎嚭鍑芥暟锛圕鎺ュ彛锛屼娇鐢?extern "C" 纭繚绗﹀彿涓€鑷达級
// ============================================================================
extern "C" {
    CAE_PLUGIN_EXPORT IPlugin* createPlugin();
    CAE_PLUGIN_EXPORT void destroyPlugin(IPlugin* plugin);
}
