#pragma once
#ifdef CAE_ENABLE_GUI

#include "core/ActionManager.h"
#include <QObject>
#include <QAction>
#include <unordered_map>
#include <string>

/// @brief QAction <-> IAction 双向桥接
///
/// 负责将 Qt 信号（QAction::triggered）转换为 ActionManager::invoke(id) 调用，
/// 确保 GUI 菜单点击与 Python/gRPC 调用走完全相同的执行路径。
class UIActionBridge : public QObject {
    Q_OBJECT
public:
    explicit UIActionBridge(QObject* parent = nullptr);
    ~UIActionBridge() override = default;

    /// @brief 为 ActionDescriptor 创建对应的 QAction 并连接信号
    QAction* createQAction(const ActionDescriptor& desc, QObject* parent = nullptr);

    /// @brief 根据 Action ID 查找对应的 QAction
    QAction* findQAction(const std::string& id) const;

    /// @brief 更新 QAction 的快捷键（setShortcut 后调用）
    void updateShortcut(const std::string& id, const std::string& shortcut);

    /// @brief 刷新 QAction 的 enabled/checked 状态
    void refreshActionState(const std::string& id);

private slots:
    void onQActionTriggered();

private:
    std::unordered_map<std::string, QAction*> qactionMap_;
};

#endif // CAE_ENABLE_GUI
