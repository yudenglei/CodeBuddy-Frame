#ifdef CAE_ENABLE_GUI
#include "ui/UIActionBridge.h"
#include "core/ActionManager.h"
#include <QAction>
#include <QKeySequence>
#include <iostream>

UIActionBridge::UIActionBridge(QObject* parent)
    : QObject(parent)
{}

QAction* UIActionBridge::createQAction(const ActionDescriptor& desc, QObject* parent) {
    // 若已存在则返回并更新属性
    auto it = qactionMap_.find(desc.id);
    if (it != qactionMap_.end()) {
        QAction* existing = it->second;
        existing->setText(QString::fromStdString(desc.label));
        if (!desc.shortcut.empty()) {
            existing->setShortcut(QKeySequence(QString::fromStdString(desc.shortcut)));
        }
        if (!desc.tooltip.empty()) {
            existing->setToolTip(QString::fromStdString(desc.tooltip));
        }
        return existing;
    }

    auto* action = new QAction(QString::fromStdString(desc.label), parent ? parent : this);

    if (!desc.shortcut.empty()) {
        action->setShortcut(QKeySequence(QString::fromStdString(desc.shortcut)));
    }
    if (!desc.tooltip.empty()) {
        action->setToolTip(QString::fromStdString(desc.tooltip));
    }
    if (!desc.iconPath.empty()) {
        action->setIcon(QIcon(QString::fromStdString(desc.iconPath)));
    }

    // 将 action id 存入 QAction 的 property，方便 triggered 时获取
    action->setProperty("cae_action_id", QString::fromStdString(desc.id));

    // 连接 triggered 信号到统一处理槽
    connect(action, &QAction::triggered, this, &UIActionBridge::onQActionTriggered);

    qactionMap_[desc.id] = action;
    return action;
}

QAction* UIActionBridge::findQAction(const std::string& id) const {
    auto it = qactionMap_.find(id);
    return (it != qactionMap_.end()) ? it->second : nullptr;
}

void UIActionBridge::updateShortcut(const std::string& id, const std::string& shortcut) {
    auto* action = findQAction(id);
    if (action) {
        action->setShortcut(QKeySequence(QString::fromStdString(shortcut)));
    }
}

void UIActionBridge::refreshActionState(const std::string& id) {
    auto* qaction = findQAction(id);
    if (!qaction) return;

    // 从 ActionManager 获取最新状态（如果有对应 IAction 实现）
    // 本框架中 ActionDescriptor 使用 callback 模式，enabled/checked 由插件控制
    // 此处预留扩展点
}

void UIActionBridge::onQActionTriggered() {
    auto* action = qobject_cast<QAction*>(sender());
    if (!action) return;

    QString id = action->property("cae_action_id").toString();
    if (id.isEmpty()) return;

    // 构建调用上下文（来源为GUI模式）
    ActionContext ctx;
    ctx.sourceMode = RunMode::GUI;

    try {
        ActionManager::instance().invoke(id.toStdString(), ctx);
    } catch (const std::exception& e) {
        std::cerr << "[UIActionBridge] invoke failed: " << e.what() << "\n";
    }
}

#endif // CAE_ENABLE_GUI
