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
    // 设置主布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(tabWidget_);
    
    // 设置Tab样式 - ANSYS风格
    tabWidget_->setDocumentMode(true);
    tabWidget_->setTabsClosable(false);
    tabWidget_->setMovable(false);
    
    // 设置样式表
    QString style = R"(
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
    // 注册默认的ANSYS风格Tabs
    registerTab(RibbonTabDef("File", "文件", 0));
    registerTab(RibbonTabDef("Draw", "绘制", 10));
    registerTab(RibbonTabDef("Modeler", "建模", 20));
    registerTab(RibbonTabDef("HFSS", "仿真", 30));
    registerTab(RibbonTabDef("Tools", "工具", 100));
}

void RibbonBar::registerTab(const RibbonTabDef& tab) {
    if (tabWidgets_.contains(tab.id)) {
        return; // 已存在
    }
    
    // 创建Tab内容容器
    QWidget* container = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(8);
    
    // 添加工具栏区域
    QScrollArea* scrollArea = new QScrollArea(container);
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
        return; // 已存在
    }
    
    // 获取Tab的布局
    QWidget* container = tabWidgets_[group.tabId];
    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(container->layout());
    QScrollArea* scrollArea = qobject_cast<QScrollArea*>(mainLayout->itemAt(0)->widget());
    QWidget* toolbarsContainer = scrollArea->widget();
    QHBoxLayout* toolbarsLayout = qobject_cast<QHBoxLayout*>(toolbarsContainer->layout());
    
    // 创建Group容器
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
    
    // Group标题
    QLabel* titleLabel = new QLabel(group.title, groupWidget);
    titleLabel->setStyleSheet(R"(
        QLabel {
            font-size: 10px;
            color: #666666;
            font-weight: bold;
        }
    )");
    groupLayout->addWidget(titleLabel);
    
    // 添加工具栏
    QToolBar* toolbar = new QToolBar(groupWidget);
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
    
    // 插入到Group容器之前（添加分隔符和Group）
    int insertPos = toolbarsLayout->count() - 1; // 在stretch之前
    toolbarsLayout->insertWidget(insertPos, groupWidget);
    
    // 保存引用
    if (!groups_.contains(group.tabId)) {
        groups_[group.tabId] = QMap<QString, QWidget*>();
    }
    groups_[group.tabId][group.id] = groupWidget;
    groupToolbars_[groupKey] = toolbar;
    
    qDebug() << "[RibbonBar] Registered group:" << group.id << "in tab:" << group.tabId;
}

void RibbonBar::registerAction(const ActionDescriptor& desc) {
    // 检查是否有Ribbon配置
    if (desc.ribbon.tabId.isEmpty()) {
        return; // 没有Ribbon配置
    }
    
    // 确保Tab存在
    if (!tabWidgets_.contains(desc.ribbon.tabId)) {
        registerTab(RibbonTabDef(desc.ribbon.tabId, desc.ribbon.tabTitle, desc.ribbon.tabOrder));
    }
    
    // 确保Group存在
    QString groupKey = desc.ribbon.tabId + "/" + desc.ribbon.groupId;
    if (!groupToolbars_.contains(groupKey)) {
        registerGroup(RibbonGroupDef(desc.ribbon.groupId, desc.ribbon.tabId, 
                                     desc.ribbon.groupTitle, desc.ribbon.groupOrder));
    }
    
    // 创建QAction
    QAction* action = new QAction(this);
    action->setText(desc.label);
    action->setToolTip(desc.tooltip.isEmpty() ? desc.label : desc.tooltip);
    action->setStatusTip(desc.tooltip);
    
    if (!desc.iconPath.isEmpty()) {
        // 尝试加载图标
        QIcon icon(desc.iconPath.c_str());
        if (!icon.isNull()) {
            action->setIcon(icon);
        }
    }
    
    if (!desc.shortcut.isEmpty()) {
        action->setShortcut(QKeySequence(desc.shortcut.c_str()));
    }
    
    // 设置按钮样式
    QToolBar* toolbar = groupToolbars_[groupKey];
    if (toolbar) {
        toolbar->addAction(action);
    }
    
    // 保存映射
    actionMap_[desc.id.c_str()] = action;
    
    // 连接信号
    connect(action, &QAction::triggered, this, [this, desc]() {
        emit actionTriggered(desc.id.c_str());
    });
    
    qDebug() << "[RibbonBar] Registered action:" << desc.id << "to group:" << desc.ribbon.groupId;
}

void RibbonBar::refresh() {
    // 重新布局（如果需要）
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
        // 注册默认Tabs
        ribbonBar_->registerTab(RibbonTabDef("File", "文件", 0));
        ribbonBar_->registerTab(RibbonTabDef("Draw", "绘制", 10));
        ribbonBar_->registerTab(RibbonTabDef("Modeler", "建模", 20));
        ribbonBar_->registerTab(RibbonTabDef("HFSS", "仿真", 30));
        ribbonBar_->registerTab(RibbonTabDef("Tools", "工具", 100));
        
        // 注册默认Groups
        // File Tab
        ribbonBar_->registerGroup(RibbonGroupDef("File", "File", "文件", 0));
        
        // Draw Tab
        ribbonBar_->registerGroup(RibbonGroupDef("Select", "Draw", "选择", 0));
        ribbonBar_->registerGroup(RibbonGroupDef("Primitives", "Draw", "基本体", 10));
        ribbonBar_->registerGroup(RibbonGroupDef("2D", "Draw", "2D图形", 20));
        ribbonBar_->registerGroup(RibbonGroupDef("Transform", "Draw", "变换", 30));
        
        // Modeler Tab
        ribbonBar_->registerGroup(RibbonGroupDef("Boolean", "Modeler", "布尔运算", 0));
        ribbonBar_->registerGroup(RibbonGroupDef("Edge", "Modeler", "边缘", 10));
        ribbonBar_->registerGroup(RibbonGroupDef("Measure", "Modeler", "测量", 20));
        
        // HFSS Tab
        ribbonBar_->registerGroup(RibbonGroupDef("Simulation", "HFSS", "仿真", 0));
        ribbonBar_->registerGroup(RibbonGroupDef("Analysis", "HFSS", "分析", 10));
        
        // 注册到ActionManager的Observer
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
    
    // 从ActionManager获取所有已注册的Action
    auto actions = ActionManager::instance().listActions();
    for (const auto& desc : actions) {
        onActionRegistered(desc);
    }
}

void RibbonBuilder::onActionRegistered(const ActionDescriptor& desc) {
    if (!ribbonBar_) return;
    
    // 检查是否有Ribbon配置
    if (!desc.ribbon.tabId.isEmpty()) {
        ribbonBar_->registerAction(desc);
    }
}

#endif // CAE_ENABLE_GUI
