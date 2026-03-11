/// @file main_gui.cpp
/// @brief GUI模式入口（QApplication + MainWindow + PluginManager）

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


// PluginManager 工厂函数（在 PluginManager.cpp 中定义）
std::unique_ptr<IPluginManager> createPluginManager();

/// @brief 辅助函数：尝试将IPlugin转换为IUIPlugin并调用setupUI
void setupUIPlugin(IPlugin* plugin, MainWindow* mainWindow) {
    if (!plugin || !mainWindow) return;
    
    // 使用dynamic_cast尝试转换为IUIPlugin
    // 这要求插件同时继承IPlugin和IUIPlugin
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

    // 设置深色主题（Fusion风格）
    app.setStyle(QStyleFactory::create("Fusion"));
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

    // 创建主窗口（内部创建 MenuBuilder，注册 ActionManager Observer）
    MainWindow mainWindow;

    // =========================================================================
    // 插件加载流程
    // =========================================================================
    
    // 1. 创建插件管理器
    auto pluginMgr = createPluginManager();
    
    // 2. 设置插件目录 - 相对于可执行文件
    // 开发模式: [build]/bin/plugins/
    // 安装模式: [安装目录]/bin/plugins/
    QString pluginsDir = QCoreApplication::applicationDirPath() + "/plugins";
    
    // 如果开发模式目录不存在，尝试备用路径
    QDir dir(pluginsDir);
    if (!dir.exists()) {
        // 尝试相对于工作目录
        pluginsDir = QCoreApplication::applicationDirPath() + "/../plugins";
        dir.setPath(pluginsDir);
        if (!dir.exists()) {
            pluginsDir = QCoreApplication::applicationDirPath() + "/../../plugins";
        }
    }
    
    std::cout << "[GUI] Plugin directory: " << pluginsDir.toStdString() << "\n";
    
    // 3. 发现插件
    if (!pluginMgr->discover(pluginsDir.toStdString())) {
        std::cout << "[GUI] No plugins found or loaded\n";
    }
    
    // 4. 初始化插件（传入GUI模式）
    pluginMgr->initializeAll(RunMode::GUI);
    
    // =========================================================================
    // 关键步骤：为每个UI/HYBRID插件调用setupUI
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
