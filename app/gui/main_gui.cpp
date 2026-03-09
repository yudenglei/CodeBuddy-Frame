/// @file main_gui.cpp
/// @brief GUI模式入口（QApplication + MainWindow + PluginManager）

#ifdef CAE_ENABLE_GUI

#include <QApplication>
#include <QStyleFactory>
#include "ui/MainWindow.h"
#include "core/ActionManager.h"
#include "core/RunMode.h"

// PluginManager 工厂函数（在 PluginManager.cpp 中定义）
#include <memory>
class IPluginManager;
std::unique_ptr<IPluginManager> createPluginManager();

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

    // 加载插件（默认目录：可执行文件旁的 plugins/ 目录）
    auto pluginMgr = createPluginManager();
    QString pluginsDir = QCoreApplication::applicationDirPath() + "/plugins";
    pluginMgr->discover(pluginsDir.toStdString());
    pluginMgr->initializeAll(RunMode::GUI);

    mainWindow.show();
    return app.exec();
}

#else
#error "main_gui.cpp requires CAE_ENABLE_GUI to be defined. Use app/headless/main_headless.cpp for headless mode."
#endif
