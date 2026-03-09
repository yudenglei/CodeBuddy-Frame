#pragma once
#ifdef CAE_ENABLE_GUI

#include "core/ActionManager.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <unordered_map>
#include <string>
#include <memory>

/// @brief 菜单自动构建器
///
/// 监听 ActionManager::onActionRegistered 事件，
/// 根据 ActionDescriptor::menuPath 自动在 QMenuBar 中创建/定位 QMenu 并插入 QAction。
///
/// menuPath 格式约定（与 KLayout menu descriptor 类似）：
///   "desktop_menu"                     → Desktop菜单末尾
///   "desktop_menu/file_group"          → Desktop菜单 File组末尾
///   "view_menu/before:zoom_out_action" → View菜单中zoom_out_action前插入
///   "---"                              → 当前菜单末尾插入分隔符
class MenuBuilder : public QObject {
    Q_OBJECT
public:
    explicit MenuBuilder(QMenuBar* menuBar, QObject* parent = nullptr);
    ~MenuBuilder() override = default;

    /// @brief 预建顶层菜单（保证顺序：Desktop/View/Layout/Tools）
    void buildTopLevelMenus();

    /// @brief 将一个ActionDescriptor插入到对应菜单位置（供Observer回调）
    void insertAction(const ActionDescriptor& desc);

    /// @brief 获取顶层菜单（供外部查询）
    QMenu* getTopMenu(const std::string& displayName) const;

private:
    /// @brief 解析 menuPath，找到或创建对应的 QMenu
    QMenu* resolveMenu(const std::string& menuPath);

    /// @brief 在 QMenu 中按 insertPosition 定位插入 QAction
    void insertIntoMenu(QMenu* menu, QAction* action, const std::string& insertPos);

    QMenuBar* menuBar_{nullptr};
    /// 顶层菜单表：displayName → QMenu*
    std::unordered_map<std::string, QMenu*> topMenus_;
    /// 所有已创建的 QAction 表：actionId → QAction*（用于定位 before:/after: 插入）
    std::unordered_map<std::string, QAction*> actionMap_;
};

#endif // CAE_ENABLE_GUI
