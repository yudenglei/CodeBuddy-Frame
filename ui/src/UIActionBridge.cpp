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
    // 鑻ュ凡瀛樺湪鍒欒繑鍥炲苟鏇存柊灞炴€?
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

    // 灏?action id 瀛樺叆 QAction 鐨?property锛屾柟渚?triggered 鏃惰幏鍙?
    action->setProperty("cae_action_id", QString::fromStdString(desc.id));

    // 杩炴帴 triggered 淇″彿鍒扮粺涓€澶勭悊妲?
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

    // 浠?ActionManager 鑾峰彇鏈€鏂扮姸鎬侊紙濡傛灉鏈夊搴?IAction 瀹炵幇锛?
    // 鏈鏋朵腑 ActionDescriptor 浣跨敤 callback 妯″紡锛宔nabled/checked 鐢辨彃浠舵帶鍒?
    // 姝ゅ棰勭暀鎵╁睍鐐?
}

void UIActionBridge::onQActionTriggered() {
    auto* action = qobject_cast<QAction*>(sender());
    if (!action) return;

    QString id = action->property("cae_action_id").toString();
    if (id.isEmpty()) return;

    // 鏋勫缓璋冪敤涓婁笅鏂囷紙鏉ユ簮涓篏UI妯″紡锛?
    ActionContext ctx;
    ctx.sourceMode = RunMode::GUI;

    try {
        ActionManager::instance().invoke(id.toStdString(), ctx);
    } catch (const std::exception& e) {
        std::cerr << "[UIActionBridge] invoke failed: " << e.what() << "\n";
    }
}

#endif // CAE_ENABLE_GUI
