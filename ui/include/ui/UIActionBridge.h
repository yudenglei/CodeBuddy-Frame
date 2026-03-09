#pragma once
#ifdef CAE_ENABLE_GUI

#include "core/ActionManager.h"
#include <QObject>
#include <QAction>
#include <unordered_map>
#include <string>

/// @brief QAction <-> ActionManager 双向桥接
/// QAction::triggered → ActionManager::invoke(id) 保证与Python/gRPC走相同路径
class UIActionBridge : public QObject {
    Q_OBJECT
public:
    explicit UIActionBridge(QObject* parent = nullptr);
    ~UIActionBridge() override = default;
    QAction* createQAction(const ActionDescriptor& desc, QObject* parent = nullptr);
    QAction* findQAction(const std::string& id) const;
    void updateShortcut(const std::string& id, const std::string& shortcut);
private slots:
    void onQActionTriggered();
private:
    std::unordered_map<std::string, QAction*> qactionMap_;
};

#endif // CAE_ENABLE_GUI
