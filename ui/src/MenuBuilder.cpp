#ifdef CAE_ENABLE_GUI
#include "ui/MenuBuilder.h"
#include "ui/UIActionBridge.h"
#include "core/MenuDescriptor.h"
#include <QAction>
#include <iostream>

static UIActionBridge* gBridge = nullptr;

MenuBuilder::MenuBuilder(QMenuBar* menuBar, QObject* parent)
    : QObject(parent), menuBar_(menuBar)
{
    if (!gBridge) gBridge = new UIActionBridge(this);
}

void MenuBuilder::buildTopLevelMenus() {
    struct TopMenu { const char* path; const char* title; };
    const TopMenu menus[] = {
        {"desktop_menu", "&Desktop"},
        {"view_menu",    "&View"},
        {"layout_menu",  "&Layout"},
        {"tools_menu",   "&Tools"},
    };
    for (const auto& m : menus) {
        QMenu* menu = menuBar_->addMenu(m.title);
        topMenus_[m.path] = menu;
        topMenus_[MenuDescriptor::toDisplayName(m.path)] = menu;
    }
}

void MenuBuilder::insertAction(const ActionDescriptor& desc) {
    if (desc.menuPath.empty() || desc.menuPath == "---") return;
    QMenu* targetMenu = resolveMenu(desc.menuPath);
    if (!targetMenu) { std::cerr << "[MenuBuilder] Cannot resolve: " << desc.menuPath << "\n"; return; }
    QAction* action = gBridge->createQAction(desc, targetMenu);
    actionMap_[desc.id] = action;
    auto info = MenuDescriptor::parse(desc.menuPath);
    insertIntoMenu(targetMenu, action, info.insertPosition);
}

QMenu* MenuBuilder::resolveMenu(const std::string& menuPath) {
    auto info = MenuDescriptor::parse(menuPath);
    if (info.isSeparator || info.menuHierarchy.empty()) return nullptr;
    const std::string& topName = info.menuHierarchy[0];
    QMenu* current = nullptr;
    auto it = topMenus_.find(topName);
    if (it != topMenus_.end()) {
        current = it->second;
    } else {
        current = menuBar_->addMenu(QString::fromStdString(topName));
        topMenus_[topName] = current;
    }
    for (size_t i = 1; i < info.menuHierarchy.size(); ++i) {
        const std::string& subName = info.menuHierarchy[i];
        QMenu* sub = nullptr;
        for (QAction* a : current->actions())
            if (a->menu() && a->text() == QString::fromStdString(subName)) { sub = a->menu(); break; }
        if (!sub) sub = current->addMenu(QString::fromStdString(subName));
        current = sub;
    }
    return current;
}

void MenuBuilder::insertIntoMenu(QMenu* menu, QAction* action, const std::string& insertPos) {
    if (insertPos.empty()) { menu->addAction(action); return; }
    if (insertPos.substr(0, 7) == "before:") {
        auto it = actionMap_.find(insertPos.substr(7));
        if (it != actionMap_.end()) { menu->insertAction(it->second, action); return; }
    } else if (insertPos.substr(0, 6) == "after:") {
        auto it = actionMap_.find(insertPos.substr(6));
        if (it != actionMap_.end()) {
            const QList<QAction*> acts = menu->actions();
            int idx = acts.indexOf(it->second);
            if (idx >= 0 && idx + 1 < acts.size()) { menu->insertAction(acts[idx + 1], action); return; }
        }
    }
    menu->addAction(action);
}

QMenu* MenuBuilder::getTopMenu(const std::string& displayName) const {
    auto it = topMenus_.find(displayName);
    return (it != topMenus_.end()) ? it->second : nullptr;
}

#endif // CAE_ENABLE_GUI
