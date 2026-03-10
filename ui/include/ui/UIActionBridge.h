#pragma once
#ifdef CAE_ENABLE_GUI

#include "core/ActionManager.h"
#include <QObject>
#include <QAction>
#include <unordered_map>
#include <string>

/// @brief QAction <-> IAction 鍙屽悜妗ユ帴
///
/// 璐熻矗灏?Qt 淇″彿锛圦Action::triggered锛夎浆鎹负 ActionManager::invoke(id) 璋冪敤锛?
/// 纭繚 GUI 鑿滃崟鐐瑰嚮涓?Python/gRPC 璋冪敤璧板畬鍏ㄧ浉鍚岀殑鎵ц璺緞銆?
class UIActionBridge : public QObject {
    Q_OBJECT
public:
    explicit UIActionBridge(QObject* parent = nullptr);
    ~UIActionBridge() override = default;

    /// @brief 涓?ActionDescriptor 鍒涘缓瀵瑰簲鐨?QAction 骞惰繛鎺ヤ俊鍙?
    QAction* createQAction(const ActionDescriptor& desc, QObject* parent = nullptr);

    /// @brief 鏍规嵁 Action ID 鏌ユ壘瀵瑰簲鐨?QAction
    QAction* findQAction(const std::string& id) const;

    /// @brief 鏇存柊 QAction 鐨勫揩鎹烽敭锛坰etShortcut 鍚庤皟鐢級
    void updateShortcut(const std::string& id, const std::string& shortcut);

    /// @brief 鍒锋柊 QAction 鐨?enabled/checked 鐘舵€?
    void refreshActionState(const std::string& id);

private slots:
    void onQActionTriggered();

private:
    std::unordered_map<std::string, QAction*> qactionMap_;
};

#endif // CAE_ENABLE_GUI
