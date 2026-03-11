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
    // ANSYS Electronics Desktop 风格顶层菜单
    struct TopMenu { const char* path; const char* title; };
    const TopMenu menus[] = {
        {"File", "&File"},
        {"Edit", "&Edit"},
        {"View", "&View"},
        {"Project", "&Project"},
        {"Draw", "&Draw"},
        {"Modeler", "&Modeler"},
        {"HFSS", "HFSS"},
        {"Tools", "&Tools"},
        {"Window", "&Window"},
        {"Help", "&Help"},
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

    // 分隔符处理
    if (desc.menuPath == "---") {
        // 独立分隔符需要在各个菜单中单独处理
        return;
    }

    // 解析menuPath获取目标菜单
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
    // 先检查缓存
    auto it = menuPathCache_.find(menuPath);
    if (it != menuPathCache_.end()) {
        return it->second;
    }

    // 解析路径
    QList<QMenu*> menus = resolveMenuPath(menuPath);
    if (menus.isEmpty()) {
        return nullptr;
    }

    // 返回最后一级菜单
    QMenu* result = menus.last();
    menuPathCache_[menuPath] = result;
    return result;
}

QList<QMenu*> MenuBuilder::resolveMenuPath(const std::string& menuPath) {
    QList<QMenu*> result;
    
    // 解析路径段
    std::vector<std::string> segments;
    std::string current;
    for (char c : menuPath) {
        if (c == '/') {
            if (!current.empty()) {
                segments.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }
    if (!current.empty()) {
        // 排除末尾的 before:/after:/--- 等标记
        if (current.find("before:") != 0 && 
            current.find("after:") != 0 &&
            current != "---") {
            segments.push_back(current);
        }
    }

    if (segments.empty()) {
        return result;
    }

    // 从顶层菜单开始
    QString topKey = QString::fromStdString(segments[0]).toLower();
    QMenu* currentMenu = nullptr;

    // 查找顶层菜单
    auto topIt = topMenus_.find(topKey.toStdString());
    if (topIt != topMenus_.end()) {
        currentMenu = topIt->second;
        result.append(currentMenu);
    } else {
        // 动态创建新的顶层菜单
        currentMenu = menuBar_->addMenu(QString::fromStdString(segments[0]));
        topMenus_[segments[0]] = currentMenu;
        topMenus_[topKey.toStdString()] = currentMenu;
        result.append(currentMenu);
    }

    // 逐级查找或创建子菜单
    for (size_t i = 1; i < segments.size(); ++i) {
        QString segment = QString::fromStdString(segments[i]);
        
        // 查找现有子菜单
        QMenu* subMenu = nullptr;
        for (QAction* a : currentMenu->actions()) {
            if (a->menu() && a->text() == segment) {
                subMenu = a->menu();
                break;
            }
        }

        // 如果不存在，创建新的子菜单
        if (!subMenu) {
            subMenu = currentMenu->addMenu(segment);
        }

        currentMenu = subMenu;
        result.append(currentMenu);
    }

    return result;
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
