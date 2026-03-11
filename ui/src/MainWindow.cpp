#ifdef CAE_ENABLE_GUI
#include "ui/MainWindow.h"
#include "ui/MenuBuilder.h"
#include "ui/RibbonBar.h"
#include "core/ActionManager.h"
#include <QCloseEvent>
#include <QDockWidget>
#include <QLabel>
#include <QWidget>
#include <QStatusBar>
#include <QApplication>
#include <QMenuBar>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , quickToolBar_(nullptr)
    , statusLabel_(nullptr)
{
    setWindowTitle("CAE PCB Designer");
    setMinimumSize(1280, 800);

    setupMenuBar();
    setupQuickToolBar();
    setupRibbonBar();
    setupStatusBar();
    setupCentralWidget();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupMenuBar() {
    menuBuilder_ = std::make_unique<MenuBuilder>(menuBar(), this);
    // 预建顶层菜单（ANSYS风格）
    menuBuilder_->buildTopLevelMenus();

    // 注册 Observer：插件加载时注册的 Action 自动插入菜单
    ActionManager::instance().onActionRegistered(
        [this](const ActionDescriptor& desc) {
            menuBuilder_->insertAction(desc);
        }
    );
}

void MainWindow::setupQuickToolBar() {
    // 快捷工具栏（传统工具栏，位于菜单栏下方）
    quickToolBar_ = addToolBar("Quick Access");
    quickToolBar_->setMovable(false);
    quickToolBar_->setIconSize(QSize(24, 24));
    quickToolBar_->setStyleSheet(R"(
        QToolBar {
            background: #F0F0F0;
            border-bottom: 1px solid #D9D9D9;
            spacing: 2px;
            padding: 2px;
        }
        QToolButton {
            background: transparent;
            border: none;
            border-radius: 3px;
            padding: 4px;
        }
        QToolButton:hover {
            background: #E0E0E0;
        }
        QToolButton:pressed {
            background: #D0D0D0;
        }
    )");
    
    // 添加默认快捷操作
    QAction* saveAction = quickToolBar_->addAction(QIcon(), "Save");
    saveAction->setToolTip("Save (Ctrl+S)");
    quickToolBar_->addSeparator();
    
    QAction* undoAction = quickToolBar_->addAction(QIcon(), "Undo");
    undoAction->setToolTip("Undo (Ctrl+Z)");
    
    QAction* redoAction = quickToolBar_->addAction(QIcon(), "Redo");
    redoAction->setToolTip("Redo (Ctrl+Y)");
    quickToolBar_->addSeparator();
    
    QAction* deleteAction = quickToolBar_->addAction(QIcon(), "Delete");
    deleteAction->setToolTip("Delete (Del)");
}

void MainWindow::setupRibbonBar() {
    // Ribbon工具栏（ANSYS风格）
    ribbonBar_ = std::make_unique<RibbonBar>(this);
    
    // 将Ribbon放在快捷工具栏下方
    // 注意：我们需要创建一个容器
    // 实际上，RibbonBar已经是一个独立widget
    // 这里我们通过setCentralWidget的替代方案
    
    // 创建主容器
    QWidget* centralContainer = new QWidget(this);
    QVBoxLayout* containerLayout = new QVBoxLayout(centralContainer);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->setSpacing(0);
    
    // 添加Ribbon（替代原来的菜单栏+工具栏布局）
    containerLayout->addWidget(ribbonBar_.get());
    
    // 添加中心区域
    auto* central = new QWidget(this);
    auto* label = new QLabel("PCB Layout View\n(Plugin area)", central);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(
        "background:#1e1e2e; color:#cdd6f4; font-size:18px; border:1px solid #45475a;");
    auto* layout = new QVBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(label);
    central->setLayout(layout);
    containerLayout->addWidget(central, 1);
    
    setCentralWidget(centralContainer);
}

void MainWindow::setupStatusBar() {
    statusLabel_ = new QLabel("Ready", this);
    statusBar()->addWidget(statusLabel_);
    statusBar()->showMessage("CAE PCB Designer - Ready");
}

void MainWindow::setupCentralWidget() {
    // 中心区域已在setupRibbonBar中设置
}

QMenu* MainWindow::getMenu(const std::string& menuName) const {
    return menuBuilder_ ? menuBuilder_->getTopMenu(menuName) : nullptr;
}

MenuBuilder* MainWindow::getMenuBuilder() const {
    return menuBuilder_.get();
}

RibbonBar* MainWindow::getRibbonBar() const {
    return ribbonBar_.get();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    // 发出关闭信号，插件可监听此信号保存数据
    ActionManager::instance().invoke("Desktop.Exit");
    event->accept();
}

#endif // CAE_ENABLE_GUI
