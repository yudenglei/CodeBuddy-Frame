#ifdef CAE_ENABLE_GUI
#include "ui/MenuBuilder.h"
#include "ui/UIActionBridge.h"
#include "core/MenuDescriptor.h"
#include <QAction>
#include <iostream>

// 全局 UIActionBridge（生命周期与 MenuBuilder 相同）
static UIActionBridge* gBridge = nullptr;

MenuBuilder::MenuBuilder(QMenuBar* menuBar, QObject* parent)
    : QObject(parent), menuBar_(menuBar)
{
    // 创建全局桥接器
    if (!gBridge) {
        gBridge = new UIActionBridge(this);
    }
}

void MenuBuilder::buildTopLevelMenus() {
    // 严格按照 HFSS 3D Layout 风格预建四个顶层菜单
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
        // 同时用 displayName 作为 key 方便 getTopMenu 查询
        topMenus_[MenuDescriptor::toDisplayName(m.path)] = menu;
    }
}

void MenuBuilder::insertAction(const ActionDescriptor& desc) {
    if (desc.menuPath.empty()) return;

    // 分隔符节点
    if (desc.menuPath == "---") return; // 独立分隔符由插件在 menuPath 中嵌入位置

    QMenu* targetMenu = resolveMenu(desc.menuPath);
    if (!targetMenu) {
        std::cerr << "[MenuBuilder] Cannot resolve menuPath: " << desc.menuPath << "\n";
        return;
    }

    // 通过 UIActionBridge 创建 QAction
    QAction* action = gBridge->createQAction(desc, targetMenu);
    actionMap_[desc.id] = action;

    // 解析插入位置
    auto info = MenuDescriptor::parse(desc.menuPath);
    insertIntoMenu(targetMenu, action, info.insertPosition);
}

QMenu* MenuBuilder::resolveMenu(const std::string& menuPath) {
    auto info = MenuDescriptor::parse(menuPath);
    if (info.isSeparator) return nullptr;
    if (info.menuHierarchy.empty()) return nullptr;

    // 第一级：顶层菜单
    const std::string& topName = info.menuHierarchy[0];

    // 先按原始 path key 找，再按 displayName 找
    QMenu* current = nullptr;
    auto it = topMenus_.find(topName);
    if (it != topMenus_.end()) {
        current = it->second;
    } else {
        // 动态创建新的顶层菜单
        current = menuBar_->addMenu(QString::fromStdString(topName));
        topMenus_[topName] = current;
    }

    // 第二级及以上：子菜单（本框架暂支持一层子菜单）
    for (size_t i = 1; i < info.menuHierarchy.size(); ++i) {
        const std::string& subName = info.menuHierarchy[i];
        QMenu* sub = nullptr;
        for (QAction* a : current->actions()) {
            if (a->menu() && a->text() == QString::fromStdString(subName)) {
                sub = a->menu();
                break;
            }
        }
        if (!sub) {
            sub = current->addMenu(QString::fromStdString(subName));
        }
        current = sub;
    }

    return current;
}

void MenuBuilder::insertIntoMenu(QMenu* menu, QAction* action, const std::string& insertPos) {
    if (insertPos.empty()) {
        menu->addAction(action);
        return;
    }

    if (insertPos.substr(0, 7) == "before:") {
        std::string refId = insertPos.substr(7);
        auto it = actionMap_.find(refId);
        if (it != actionMap_.end()) {
            menu->insertAction(it->second, action);
            return;
        }
    } else if (insertPos.substr(0, 6) == "after:") {
        std::string refId = insertPos.substr(6);
        auto it = actionMap_.find(refId);
        if (it != actionMap_.end()) {
            // Qt没有直接的insertAfter，用insertAction插入refAction之后
            const QList<QAction*> acts = menu->actions();
            int idx = acts.indexOf(it->second);
            if (idx >= 0 && idx + 1 < acts.size()) {
                menu->insertAction(acts[idx + 1], action);
            } else {
                menu->addAction(action);
            }
            return;
        }
    }

    // 找不到参考点则追加到末尾
    menu->addAction(action);
}

QMenu* MenuBuilder::getTopMenu(const std::string& displayName) const {
    auto it = topMenus_.find(displayName);
    return (it != topMenus_.end()) ? it->second : nullptr;
}

#endif // CAE_ENABLE_GUI
