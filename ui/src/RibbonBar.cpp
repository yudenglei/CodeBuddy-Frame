#ifdef CAE_ENABLE_GUI
#include "ui/RibbonBar.h"
#include "core/ActionManager.h"
#include <QDebug>
#include <QIcon>
#include <QSizePolicy>

RibbonBar::RibbonBar(QWidget* parent)
    : QWidget(parent)
    , tabWidget_(new QTabWidget(this))
{
    setupUi();
    createDefaultTabs();
}

RibbonBar::~RibbonBar() = default;

void RibbonBar::setupUi() {
    // 璁剧疆涓诲竷灞€
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(tabWidget_);
    
    // 璁剧疆Tab鏍峰紡 - ANSYS椋庢牸
    tabWidget_->setDocumentMode(true);
    tabWidget_->setTabsClosable(false);
    tabWidget_->setMovable(false);
    
    // 璁剧疆鏍峰紡琛?    QString style = R"(
        QTabWidget::pane {
            border: 1px solid #D9D9D9;
            background: white;
        }
        QTabBar::tab {
            background: #F0F0F0;
            border: 1px solid #D9D9D9;
            border-bottom: none;
            padding: 6px 16px;
            margin-right: 2px;
            font-size: 11px;
            color: #333333;
        }
        QTabBar::tab:selected {
            background: white;
            border-bottom: 2px solid #0078D7;
        }
        QTabBar::tab:hover:!selected {
            background: #E5E5E5;
        }
    )";
    tabWidget_->setStyleSheet(style);
    
    connect(tabWidget_, &QTabWidget::currentChanged, this, &RibbonBar::onTabChanged);
}

void RibbonBar::createDefaultTabs() {
    // 娉ㄥ唽榛樿鐨凙NSYS椋庢牸Tabs
    registerTab(RibbonTabDef("File", "鏂囦欢", 0));
    registerTab(RibbonTabDef("Draw", "缁樺埗", 10));
    registerTab(RibbonTabDef("Modeler", "寤烘ā", 20));
    registerTab(RibbonTabDef("HFSS", "浠跨湡", 30));
    registerTab(RibbonTabDef("Tools", "宸ュ叿", 100));
}

void RibbonBar::registerTab(const RibbonTabDef& tab) {
    if (tabWidgets_.contains(tab.id)) {
        return; // 宸插瓨鍦?    }
    
    // 鍒涘缓Tab鍐呭瀹瑰櫒
    QWidget* container = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(8);
    
    // 娣诲姞宸ュ叿鏍忓尯鍩?    QScrollArea* scrollArea = new QScrollArea(container);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setStyleSheet("QScrollArea { border: none; background: white; }");
    
    QWidget* toolbarsContainer = new QWidget(scrollArea);
    QHBoxLayout* toolbarsLayout = new QHBoxLayout(toolbarsContainer);
    toolbarsLayout->setContentsMargins(0, 0, 0, 0);
    toolbarsLayout->setSpacing(8);
    toolbarsLayout->addStretch();
    
    scrollArea->setWidget(toolbarsContainer);
    layout->addWidget(scrollArea);
    
    tabWidget_->addTab(container, tab.title);
    tabWidgets_[tab.id] = container;
    
    qDebug() << "[RibbonBar] Registered tab:" << tab.id << tab.title;
}

void RibbonBar::registerGroup(const RibbonGroupDef& group) {
    if (!tabWidgets_.contains(group.tabId)) {
        qWarning() << "[RibbonBar] Tab not found:" << group.tabId;
        return;
    }
    
    QString groupKey = group.tabId + "/" + group.id;
    if (groups_.contains(group.tabId) && groups_[group.tabId].contains(group.id)) {
        return; // 宸插瓨鍦?    }
    
    // 鑾峰彇Tab鐨勫竷灞€
    QWidget* container = tabWidgets_[group.tabId];
    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(container->layout());
    QScrollArea* scrollArea = qobject_cast<QScrollArea*>(mainLayout->itemAt(0)->widget());
    QWidget* toolbarsContainer = scrollArea->widget();
    QHBoxLayout* toolbarsLayout = qobject_cast<QHBoxLayout*>(toolbarsContainer->layout());
    
    // 鍒涘缓Group瀹瑰櫒
    QWidget* groupWidget = new QWidget(toolbarsContainer);
    groupWidget->setStyleSheet(R"(
        QWidget {
            background: #FAFAFA;
            border: 1px solid #E0E0E0;
            border-radius: 4px;
        }
    )");
    
    QVBoxLayout* groupLayout = new QVBoxLayout(groupWidget);
    groupLayout->setContentsMargins(8, 6, 8, 6);
    groupLayout->setSpacing(4);
    
    // Group鏍囬
    QLabel* titleLabel = new QLabel(group.title, groupWidget);
    titleLabel->setStyleSheet(R"(
        QLabel {
            font-size: 10px;
            color: #666666;
            font-weight: bold;
        }
    )");
    groupLayout->addWidget(titleLabel);
    
    // 娣诲姞宸ュ叿鏍?    QToolBar* toolbar = new QToolBar(groupWidget);
    toolbar->setMovable(false);
    toolbar->setFloatable(false);
    toolbar->setStyleSheet(R"(
        QToolBar {
            border: none;
            background: transparent;
            spacing: 2px;
        }
    )");
    groupLayout->addWidget(toolbar);
    
    // 鎻掑叆鍒癎roup瀹瑰櫒涔嬪墠锛堟坊鍔犲垎闅旂鍜孏roup锛?    int insertPos = toolbarsLayout->count() - 1; // 鍦╯tretch涔嬪墠
    toolbarsLayout->insertWidget(insertPos, groupWidget);
    
    // 淇濆瓨寮曠敤
    if (!groups_.contains(group.tabId)) {
        groups_[group.tabId] = QMap<QString, QWidget*>();
    }
    groups_[group.tabId][group.id] = groupWidget;
    groupToolbars_[groupKey] = toolbar;
    
    qDebug() << "[RibbonBar] Registered group:" << group.id << "in tab:" << group.tabId;
}

void RibbonBar::registerAction(const ActionDescriptor& desc) {
    // 妫€鏌ユ槸鍚︽湁Ribbon閰嶇疆
    if (desc.ribbon.tabId.isEmpty()) {
        return; // 娌℃湁Ribbon閰嶇疆
    }
    
    // 纭繚Tab瀛樺湪
    if (!tabWidgets_.contains(desc.ribbon.tabId)) {
        registerTab(RibbonTabDef(desc.ribbon.tabId, desc.ribbon.tabTitle, desc.ribbon.tabOrder));
    }
    
    // 纭繚Group瀛樺湪
    QString groupKey = desc.ribbon.tabId + "/" + desc.ribbon.groupId;
    if (!groupToolbars_.contains(groupKey)) {
        registerGroup(RibbonGroupDef(desc.ribbon.groupId, desc.ribbon.tabId, 
                                     desc.ribbon.groupTitle, desc.ribbon.groupOrder));
    }
    
    // 鍒涘缓QAction
    QAction* action = new QAction(this);
    action->setText(desc.label);
    action->setToolTip(desc.tooltip.isEmpty() ? desc.label : desc.tooltip);
    action->setStatusTip(desc.tooltip);
    
    if (!desc.iconPath.isEmpty()) {
        // 灏濊瘯鍔犺浇鍥炬爣
        QIcon icon(desc.iconPath.c_str());
        if (!icon.isNull()) {
            action->setIcon(icon);
        }
    }
    
    if (!desc.shortcut.isEmpty()) {
        action->setShortcut(QKeySequence(desc.shortcut.c_str()));
    }
    
    // 璁剧疆鎸夐挳鏍峰紡
    QToolBar* toolbar = groupToolbars_[groupKey];
    if (toolbar) {
        toolbar->addAction(action);
    }
    
    // 淇濆瓨鏄犲皠
    actionMap_[desc.id.c_str()] = action;
    
    // 杩炴帴淇″彿
    connect(action, &QAction::triggered, this, [this, desc]() {
        emit actionTriggered(desc.id.c_str());
    });
    
    qDebug() << "[RibbonBar] Registered action:" << desc.id << "to group:" << desc.ribbon.groupId;
}

void RibbonBar::refresh() {
    // 閲嶆柊甯冨眬锛堝鏋滈渶瑕侊級
    tabWidget_->update();
}

QString RibbonBar::currentTabId() const {
    int idx = tabWidget_->currentIndex();
    if (idx >= 0) {
        return tabWidget_->tabText(idx);
    }
    return QString();
}

void RibbonBar::paintEvent(QPaintEvent* event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    p.fillRect(opt.rect, QColor("#F5F5F5"));
    QWidget::paintEvent(event);
}

void RibbonBar::onTabChanged(int index) {
    qDebug() << "[RibbonBar] Tab changed to index:" << index;
}

void RibbonBar::onToolButtonTriggered() {
    QToolButton* btn = qobject_cast<QToolButton*>(sender());
    if (btn) {
        QString actionId = btn->objectName();
        if (!actionId.isEmpty()) {
            emit actionTriggered(actionId);
        }
    }
}

// ============================================================================
// RibbonBuilder
// ============================================================================

RibbonBuilder::RibbonBuilder(RibbonBar* ribbonBar, QObject* parent)
    : QObject(parent)
    , ribbonBar_(ribbonBar)
{
    if (ribbonBar_) {
        // 娉ㄥ唽榛樿Tabs
        ribbonBar_->registerTab(RibbonTabDef("File", "鏂囦欢", 0));
        ribbonBar_->registerTab(RibbonTabDef("Draw", "缁樺埗", 10));
        ribbonBar_->registerTab(RibbonTabDef("Modeler", "寤烘ā", 20));
        ribbonBar_->registerTab(RibbonTabDef("HFSS", "浠跨湡", 30));
        ribbonBar_->registerTab(RibbonTabDef("Tools", "宸ュ叿", 100));
        
        // 娉ㄥ唽榛樿Groups
        // File Tab
        ribbonBar_->registerGroup(RibbonGroupDef("File", "File", "鏂囦欢", 0));
        
        // Draw Tab
        ribbonBar_->registerGroup(RibbonGroupDef("Select", "Draw", "閫夋嫨", 0));
        ribbonBar_->registerGroup(RibbonGroupDef("Primitives", "Draw", "鍩烘湰浣?, 10));
        ribbonBar_->registerGroup(RibbonGroupDef("2D", "Draw", "2D鍥惧舰", 20));
        ribbonBar_->registerGroup(RibbonGroupDef("Transform", "Draw", "鍙樻崲", 30));
        
        // Modeler Tab
        ribbonBar_->registerGroup(RibbonGroupDef("Boolean", "Modeler", "甯冨皵杩愮畻", 0));
        ribbonBar_->registerGroup(RibbonGroupDef("Edge", "Modeler", "杈圭紭", 10));
        ribbonBar_->registerGroup(RibbonGroupDef("Measure", "Modeler", "娴嬮噺", 20));
        
        // HFSS Tab
        ribbonBar_->registerGroup(RibbonGroupDef("Simulation", "HFSS", "浠跨湡", 0));
        ribbonBar_->registerGroup(RibbonGroupDef("Analysis", "HFSS", "鍒嗘瀽", 10));
        
        // 娉ㄥ唽鍒癆ctionManager鐨凮bserver
        ActionManager::instance().onActionRegistered(
            [this](const ActionDescriptor& desc) {
                onActionRegistered(desc);
            }
        );
    }
}

RibbonBuilder::~RibbonBuilder() = default;

void RibbonBuilder::buildFromActionManager() {
    if (!ribbonBar_) return;
    
    // 浠嶢ctionManager鑾峰彇鎵€鏈夊凡娉ㄥ唽鐨凙ction
    auto actions = ActionManager::instance().listActions();
    for (const auto& desc : actions) {
        onActionRegistered(desc);
    }
}

void RibbonBuilder::onActionRegistered(const ActionDescriptor& desc) {
    if (!ribbonBar_) return;
    
    // 妫€鏌ユ槸鍚︽湁Ribbon閰嶇疆
    if (!desc.ribbon.tabId.isEmpty()) {
        ribbonBar_->registerAction(desc);
    }
}

#endif // CAE_ENABLE_GUI
