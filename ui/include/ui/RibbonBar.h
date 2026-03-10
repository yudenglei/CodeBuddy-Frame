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
// RibbonTabDef - Ribbon閫夐」鍗″畾涔?// ============================================================================

struct RibbonTabDef {
    QString id;
    QString title;
    int order;
    
    RibbonTabDef() : order(0) {}
    RibbonTabDef(const QString& id_, const QString& title_, int order_ = 0)
        : id(id_), title(title_), order(order_) {}
};

// ============================================================================
// RibbonGroupDef - Ribbon鍒嗙粍瀹氫箟
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
// RibbonBar - ANSYS椋庢牸Ribbon宸ュ叿鏍?// ============================================================================

/// @brief Ribbon宸ュ叿鏍忔帶浠讹紙鍙傝€傾NSYS Electronics Desktop椋庢牸锛?/// 
/// 鍔熻兘锛?/// - 鏀寔澶氫釜Tab椤电
/// - 姣忎釜Tab涓嬫湁澶氫釜Group鍒嗙粍
/// - 姣忎釜Group鍐呮湁宸ュ叿鏍忔寜閽?/// - 鏀寔澶у浘鏍?鏂囧瓧銆佸皬鍥炬爣+鏂囧瓧銆佷粎鍥炬爣涓夌妯″紡
class RibbonBar : public QWidget {
    Q_OBJECT
public:
    explicit RibbonBar(QWidget* parent = nullptr);
    ~RibbonBar() override;
    
    /// @brief 娉ㄥ唽涓€涓猂ibbon Tab
    void registerTab(const RibbonTabDef& tab);
    
    /// @brief 娉ㄥ唽涓€涓猂ibbon Group
    void registerGroup(const RibbonGroupDef& group);
    
    /// @brief 娉ㄥ唽Action鍒板伐鍏锋爮锛堟牴鎹甊ibbonInfo鑷姩鏀剧疆锛?    void registerAction(const ActionDescriptor& desc);
    
    /// @brief 鍒锋柊鏁翠釜Ribbon甯冨眬
    void refresh();
    
    /// @brief 鑾峰彇褰撳墠娲诲姩Tab
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
    
    // Action鍒癚Action鐨勬槧灏?    QMap<QString, QAction*> actionMap_;
};

// ============================================================================
// RibbonBuilder - 鐢ㄤ簬浠嶢ctionManager鏋勫缓Ribbon
// ============================================================================

class RibbonBuilder : public QObject {
    Q_OBJECT
public:
    explicit RibbonBuilder(RibbonBar* ribbonBar, QObject* parent = nullptr);
    ~RibbonBuilder() override;
    
    /// @brief 浠嶢ctionManager鏋勫缓鏁翠釜Ribbon
    void buildFromActionManager();
    
private:
    void onActionRegistered(const ActionDescriptor& desc);
    
    RibbonBar* ribbonBar_;
};

#endif // CAE_ENABLE_GUI
