#pragma once
#ifdef CAE_ENABLE_GUI

#include "core/ActionManager.h"
#include "core/MenuDescriptor.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <unordered_map>
#include <string>
#include <memory>

/// @brief 菜单自动构建器（支持多级子菜单）
///
/// 监听 ActionManager::onActionRegistered 事件，
/// 根据 ActionDescriptor::menuPath 自动在 QMenuBar 中创建/定位 QMenu 并插入 QAction。
///
/// menuPath 格式约定（ANSYS风格）：
///   "File"                          → File菜单末尾
///   "File/Save"                     → File菜单Save子项
///   "File/New"                      → File菜单New子项
///   "File/before:Save"              → File菜单Save之前
///   "File/after:New"                → File菜单New之后
///   "File/Save---"                  → Save之后加分隔符
///   "---"                           → 独立分隔符
class MenuBuilder : public QObject {
    Q_OBJECT
public:
    explicit MenuBuilder(QMenuBar* menuBar, QObject* parent = nullptr);
    ~MenuBuilder() override = default;

    /// @brief 预建顶层菜单（ANSYS风格：File/Edit/View/Project/Draw/Modeler/HFSS/Tools/Window/Help）
    void buildTopLevelMenus();

    /// @brief 将一个ActionDescriptor插入到对应菜单位置（供Observer回调）
    void insertAction(const ActionDescriptor& desc);

    /// @brief 获取顶层菜单（供外部查询）
    QMenu* getTopMenu(const std::string& displayName) const;

private:
    /// @brief 解析menuPath，找到或创建对应的QMenu（支持多级子菜单）
    QMenu* resolveMenu(const std::string& menuPath);

    /// @brief 解析menuPath并返回路径上的所有菜单
    QList<QMenu*> resolveMenuPath(const std::string& menuPath);

    /// @brief 在QMenu中按insertPosition定位插入QAction
    void insertIntoMenu(QMenu* menu, QAction* action, const std::string& insertPos);

    QMenuBar* menuBar_{nullptr};
    
    /// 顶层菜单表：key(如"File") → QMenu*
    std::unordered_map<std::string, QMenu*> topMenus_;
    
    /// 所有已创建的QAction表：actionId → QAction*
    std::unordered_map<std::string, QAction*> actionMap_;
    
    /// 菜单路径缓存：menuPath → QMenu*（用于快速查找）
    std::unordered_map<std::string, QMenu*> menuPathCache_;
};

#endif // CAE_ENABLE_GUI
