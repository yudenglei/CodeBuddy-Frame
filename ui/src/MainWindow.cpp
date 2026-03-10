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
    // 棰勫缓椤跺眰鑿滃崟锛圓NSYS椋庢牸锛?    menuBuilder_->buildTopLevelMenus();

    // 娉ㄥ唽 Observer锛氭彃浠跺姞杞芥椂娉ㄥ唽鐨?Action 鑷姩鎻掑叆鑿滃崟
    ActionManager::instance().onActionRegistered(
        [this](const ActionDescriptor& desc) {
            menuBuilder_->insertAction(desc);
        }
    );
}

void MainWindow::setupQuickToolBar() {
    // 蹇嵎宸ュ叿鏍忥紙浼犵粺宸ュ叿鏍忥紝浣嶄簬鑿滃崟鏍忎笅鏂癸級
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
    
    // 娣诲姞榛樿蹇嵎鎿嶄綔
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
    // Ribbon宸ュ叿鏍忥紙ANSYS椋庢牸锛?    ribbonBar_ = std::make_unique<RibbonBar>(this);
    
    // 灏哛ibbon鏀惧湪蹇嵎宸ュ叿鏍忎笅鏂?    // 娉ㄦ剰锛氭垜浠渶瑕佸垱寤轰竴涓鍣?    // 瀹為檯涓婏紝RibbonBar宸茬粡鏄竴涓嫭绔媤idget
    // 杩欓噷鎴戜滑閫氳繃setCentralWidget鐨勬浛浠ｆ柟妗?    
    // 鍒涘缓涓诲鍣?    QWidget* centralContainer = new QWidget(this);
    QVBoxLayout* containerLayout = new QVBoxLayout(centralContainer);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->setSpacing(0);
    
    // 娣诲姞Ribbon锛堟浛浠ｅ師鏉ョ殑鑿滃崟鏍?宸ュ叿鏍忓竷灞€锛?    containerLayout->addWidget(ribbonBar_.get());
    
    // 娣诲姞涓績鍖哄煙
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
    // 涓績鍖哄煙宸插湪setupRibbonBar涓缃?}

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
    // 鍙戝嚭鍏抽棴淇″彿锛屾彃浠跺彲鐩戝惉姝や俊鍙蜂繚瀛樻暟鎹?    ActionManager::instance().invoke("Desktop.Exit");
    event->accept();
}

#endif // CAE_ENABLE_GUI
