#pragma once
#ifdef CAE_ENABLE_GUI

#include "core/ActionManager.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <unordered_map>
#include <string>
#include <memory>

/// @brief 菜单自动构建器（KLayout menu descriptor 模式）
/// 监听 ActionManager::onActionRegistered，根据 menuPath 自动创建 QMenu/QAction
class MenuBuilder : public QObject {
    Q_OBJECT
public:
    explicit MenuBuilder(QMenuBar* menuBar, QObject* parent = nullptr);
    ~MenuBuilder() override = default;
    void buildTopLevelMenus();  ///< 预建 Desktop/View/Layout/Tools 四个顶层菜单
    void insertAction(const ActionDescriptor& desc);
    QMenu* getTopMenu(const std::string& displayName) const;
private:
    QMenu* resolveMenu(const std::string& menuPath);
    void insertIntoMenu(QMenu* menu, QAction* action, const std::string& insertPos);
    QMenuBar* menuBar_{nullptr};
    std::unordered_map<std::string, QMenu*> topMenus_;
    std::unordered_map<std::string, QAction*> actionMap_;
};

#endif // CAE_ENABLE_GUI
