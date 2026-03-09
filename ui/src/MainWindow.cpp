#ifdef CAE_ENABLE_GUI
#include "ui/MainWindow.h"
#include "ui/MenuBuilder.h"
#include "core/ActionManager.h"
#include <QCloseEvent>
#include <QDockWidget>
#include <QLabel>
#include <QWidget>
#include <QStatusBar>
#include <QApplication>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("CAE PCB Designer");
    setMinimumSize(1280, 800);

    setupMenuBar();
    setupStatusBar();
    setupCentralWidget();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupMenuBar() {
    menuBuilder_ = std::make_unique<MenuBuilder>(menuBar(), this);
    // 预建顶层菜单（保证 Desktop/View/Layout/Tools 顺序）
    menuBuilder_->buildTopLevelMenus();

    // 注册 Observer：插件加载时注册的 Action 自动插入菜单
    ActionManager::instance().onActionRegistered(
        [this](const ActionDescriptor& desc) {
            menuBuilder_->insertAction(desc);
        }
    );
}

void MainWindow::setupStatusBar() {
    statusLabel_ = new QLabel("Ready", this);
    statusBar()->addWidget(statusLabel_);
    statusBar()->showMessage("CAE PCB Designer - Ready");
}

void MainWindow::setupCentralWidget() {
    auto* central = new QWidget(this);
    auto* label = new QLabel("PCB Layout View\n(Plugin area)", central);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(
        "background:#1e1e2e; color:#cdd6f4; font-size:18px; border:1px solid #45475a;");
    auto* layout = new QVBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(label);
    central->setLayout(layout);
    setCentralWidget(central);
}

QMenu* MainWindow::getMenu(const std::string& menuName) const {
    return menuBuilder_ ? menuBuilder_->getTopMenu(menuName) : nullptr;
}

MenuBuilder* MainWindow::getMenuBuilder() const {
    return menuBuilder_.get();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    // 发出关闭信号，插件可监听此信号保存数据
    ActionManager::instance().invoke("Desktop.Exit");
    event->accept();
}

#endif // CAE_ENABLE_GUI
