#pragma once
#ifdef CAE_ENABLE_GUI

#include <QWidget>
#include <QTabWidget>
#include <QToolBar>
#include <QMap>
#include <QVector>
#include <QLayout>
#include <QLabel>
#include <QAction>
#include <QToolButton>
#include <QPushButton>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QStyleOption>
#include <QStyle>
#include "core/ActionManager.h"

// ============================================================================
// RibbonTabDef - Ribbon选项卡定义
// ============================================================================

struct RibbonTabDef {
    QString id;
    QString title;
    int order;
    
    RibbonTabDef() : order(0) {}
    RibbonTabDef(const QString& id_, const QString& title_, int order_ = 0)
        : id(id_), title(title_), order(order_) {}
};

// ============================================================================
// RibbonGroupDef - Ribbon分组定义
// ============================================================================

struct RibbonGroupDef {
    QString id;
    QString tabId;
    QString title;
    int order;
    
    RibbonGroupDef() : order(0) {}
    RibbonGroupDef(const QString& id_, const QString& tabId_, const QString& title_, int order_ = 0)
        : id(id_), tabId(tabId_), title(title_), order(order_) {}
};

// ============================================================================
// RibbonBar - ANSYS风格Ribbon工具栏
// ============================================================================

/// @brief Ribbon工具栏控件（参考ANSYS Electronics Desktop风格）
/// 
/// 功能：
/// - 支持多个Tab页签
/// - 每个Tab下有多个Group分组
/// - 每个Group内有工具栏按钮
/// - 支持大图标+文字、小图标+文字、仅图标三种模式
class RibbonBar : public QWidget {
    Q_OBJECT
public:
    explicit RibbonBar(QWidget* parent = nullptr);
    ~RibbonBar() override;
    
    /// @brief 注册一个Ribbon Tab
    void registerTab(const RibbonTabDef& tab);
    
    /// @brief 注册一个Ribbon Group
    void registerGroup(const RibbonGroupDef& group);
    
    /// @brief 注册Action到工具栏（根据RibbonInfo自动放置）
    void registerAction(const ActionDescriptor& desc);
    
    /// @brief 刷新整个Ribbon布局
    void refresh();
    
    /// @brief 获取当前活动Tab
    QString currentTabId() const;
    
signals:
    void actionTriggered(const QString& actionId);

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onTabChanged(int index);
    void onToolButtonTriggered();

private:
    void setupUi();
    void createDefaultTabs();
    
    QTabWidget* tabWidget_;
    QMap<QString, QWidget*> tabWidgets_;     // tabId -> container widget
    QMap<QString, QMap<QString, QWidget*>> groups_; // tabId -> (groupId -> group widget)
    QMap<QString, QToolBar*> groupToolbars_; // groupId -> toolbar
    
    // Action到QAction的映射
    QMap<QString, QAction*> actionMap_;
};

// ============================================================================
// RibbonBuilder - 用于从ActionManager构建Ribbon
// ============================================================================

class RibbonBuilder : public QObject {
    Q_OBJECT
public:
    explicit RibbonBuilder(RibbonBar* ribbonBar, QObject* parent = nullptr);
    ~RibbonBuilder() override;
    
    /// @brief 从ActionManager构建整个Ribbon
    void buildFromActionManager();
    
private:
    void onActionRegistered(const ActionDescriptor& desc);
    
    RibbonBar* ribbonBar_;
};

#endif // CAE_ENABLE_GUI
