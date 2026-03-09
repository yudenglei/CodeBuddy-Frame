#ifdef CAE_ENABLE_GUI
#include "ui/UIActionBridge.h"
#include "core/ActionManager.h"
#include <QAction>
#include <QKeySequence>
#include <iostream>

UIActionBridge::UIActionBridge(QObject* parent) : QObject(parent) {}

QAction* UIActionBridge::createQAction(const ActionDescriptor& desc, QObject* parent) {
    auto it = qactionMap_.find(desc.id);
    if (it != qactionMap_.end()) {
        it->second->setText(QString::fromStdString(desc.label));
        if (!desc.shortcut.empty()) it->second->setShortcut(QKeySequence(QString::fromStdString(desc.shortcut)));
        return it->second;
    }
    auto* action = new QAction(QString::fromStdString(desc.label), parent ? parent : this);
    if (!desc.shortcut.empty()) action->setShortcut(QKeySequence(QString::fromStdString(desc.shortcut)));
    if (!desc.tooltip.empty()) action->setToolTip(QString::fromStdString(desc.tooltip));
    if (!desc.iconPath.empty()) action->setIcon(QIcon(QString::fromStdString(desc.iconPath)));
    action->setProperty("cae_action_id", QString::fromStdString(desc.id));
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
    if (action) action->setShortcut(QKeySequence(QString::fromStdString(shortcut)));
}

void UIActionBridge::onQActionTriggered() {
    auto* action = qobject_cast<QAction*>(sender());
    if (!action) return;
    QString id = action->property("cae_action_id").toString();
    if (id.isEmpty()) return;
    ActionContext ctx;
    ctx.sourceMode = RunMode::GUI;
    try { ActionManager::instance().invoke(id.toStdString(), ctx); }
    catch (const std::exception& e) { std::cerr << "[UIActionBridge] " << e.what() << "\n"; }
}

#endif // CAE_ENABLE_GUI
