/// @file main_gui.cpp
/// @brief GUI妯″紡鍏ュ彛锛圦Application + MainWindow + PluginManager锛?
#ifdef CAE_ENABLE_GUI

#include <QApplication>
#include <QStyleFactory>
#include <QCoreApplication>
#include <QPalette>
#include <QColor>
#include "ui/MainWindow.h"
#include "ui/IUIPlugin.h"
#include "core/ActionManager.h"
#include "core/RunMode.h"
#include "core/IPluginManager.h"
#include "core/PluginMeta.h"


// PluginManager 宸ュ巶鍑芥暟锛堝湪 PluginManager.cpp 涓畾涔夛級
std::unique_ptr<IPluginManager> createPluginManager();

/// @brief 杈呭姪鍑芥暟锛氬皾璇曞皢IPlugin杞崲涓篒UIPlugin骞惰皟鐢╯etupUI
void setupUIPlugin(IPlugin* plugin, MainWindow* mainWindow) {
    if (!plugin || !mainWindow) return;
    
    // 浣跨敤dynamic_cast灏濊瘯杞崲涓篒UIPlugin
    // 杩欒姹傛彃浠跺悓鏃剁户鎵縄Plugin鍜孖UIPlugin
    IUIPlugin* uiPlugin = dynamic_cast<IUIPlugin*>(plugin);
    if (uiPlugin) {
        std::cout << "[GUI] Calling setupUI for UI plugin\n";
        uiPlugin->setupUI(mainWindow);
    }
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("CAE PCB Designer");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("CAE Corp");

    // 璁剧疆娣辫壊涓婚锛團usion椋庢牸锛?    app.setStyle(QStyleFactory::create("Fusion"));
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window,          QColor(30, 30, 46));
    darkPalette.setColor(QPalette::WindowText,       QColor(205, 214, 244));
    darkPalette.setColor(QPalette::Base,             QColor(24, 24, 37));
    darkPalette.setColor(QPalette::AlternateBase,    QColor(30, 30, 46));
    darkPalette.setColor(QPalette::ToolTipBase,      QColor(30, 30, 46));
    darkPalette.setColor(QPalette::ToolTipText,      QColor(205, 214, 244));
    darkPalette.setColor(QPalette::Text,             QColor(205, 214, 244));
    darkPalette.setColor(QPalette::Button,           QColor(49, 50, 68));
    darkPalette.setColor(QPalette::ButtonText,       QColor(205, 214, 244));
    darkPalette.setColor(QPalette::Highlight,        QColor(137, 180, 250));
    darkPalette.setColor(QPalette::HighlightedText,  QColor(30, 30, 46));
    app.setPalette(darkPalette);

    // 鍒涘缓涓荤獥鍙ｏ紙鍐呴儴鍒涘缓 MenuBuilder锛屾敞鍐?ActionManager Observer锛?    MainWindow mainWindow;

    // =========================================================================
    // 鎻掍欢鍔犺浇娴佺▼
    // =========================================================================
    
    // 1. 鍒涘缓鎻掍欢绠＄悊鍣?    auto pluginMgr = createPluginManager();
    
    // 2. 璁剧疆鎻掍欢鐩綍 - 鐩稿浜庡彲鎵ц鏂囦欢
    // 寮€鍙戞ā寮? [build]/bin/plugins/
    // 瀹夎妯″紡: [瀹夎鐩綍]/bin/plugins/
    QString pluginsDir = QCoreApplication::applicationDirPath() + "/plugins";
    
    // 濡傛灉寮€鍙戞ā寮忕洰褰曚笉瀛樺湪锛屽皾璇曞鐢ㄨ矾寰?    QDir dir(pluginsDir);
    if (!dir.exists()) {
        // 灏濊瘯鐩稿浜庡伐浣滅洰褰?        pluginsDir = QCoreApplication::applicationDirPath() + "/../plugins";
        dir.setPath(pluginsDir);
        if (!dir.exists()) {
            pluginsDir = QCoreApplication::applicationDirPath() + "/../../plugins";
        }
    }
    
    std::cout << "[GUI] Plugin directory: " << pluginsDir.toStdString() << "\n";
    
    // 3. 鍙戠幇鎻掍欢
    if (!pluginMgr->discover(pluginsDir.toStdString())) {
        std::cout << "[GUI] No plugins found or loaded\n";
    }
    
    // 4. 鍒濆鍖栨彃浠讹紙浼犲叆GUI妯″紡锛?    pluginMgr->initializeAll(RunMode::GUI);
    
    // =========================================================================
    // 鍏抽敭姝ラ锛氫负姣忎釜UI/HYBRID鎻掍欢璋冪敤setupUI
    // =========================================================================
    for (auto* plugin : pluginMgr->getAllPlugins()) {
        if (!plugin) continue;
        
        PluginMeta meta = plugin->getMeta();
        if (meta.type == PluginType::UI_ONLY || meta.type == PluginType::HYBRID) {
            std::cout << "[GUI] Setting up UI for plugin: " << meta.name << "\n";
            setupUIPlugin(plugin, &mainWindow);
        }
    }

    mainWindow.show();
    return app.exec();
}


#else
#error "main_gui.cpp requires CAE_ENABLE_GUI to be defined. Use app/headless/main_headless.cpp for headless mode."
#endif
