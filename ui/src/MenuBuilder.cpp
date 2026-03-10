#ifdef CAE_ENABLE_GUI
#include "ui/MenuBuilder.h"
#include "ui/UIActionBridge.h"
#include "core/MenuDescriptor.h"
#include <QAction>
#include <iostream>

// 鍏ㄥ眬 UIActionBridge锛堢敓鍛藉懆鏈熶笌 MenuBuilder 鐩稿悓锛?static UIActionBridge* gBridge = nullptr;

MenuBuilder::MenuBuilder(QMenuBar* menuBar, QObject* parent)
    : QObject(parent), menuBar_(menuBar)
{
    // 鍒涘缓鍏ㄥ眬妗ユ帴鍣?    if (!gBridge) {
        gBridge = new UIActionBridge(this);
    }
}

void MenuBuilder::buildTopLevelMenus() {
    // ANSYS Electronics Desktop 椋庢牸椤跺眰鑿滃崟
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
        // 鍚屾椂鐢?displayName 浣滀负 key 鏂逛究 getTopMenu 鏌ヨ
        topMenus_[MenuDescriptor::toDisplayName(m.path)] = menu;
    }
}

void MenuBuilder::insertAction(const ActionDescriptor& desc) {
    if (desc.menuPath.empty()) return;

    // 鍒嗛殧绗﹀鐞?    if (desc.menuPath == "---") {
        // 鐙珛鍒嗛殧绗﹂渶瑕佸湪鍚勪釜鑿滃崟涓崟鐙鐞?        return;
    }

    // 瑙ｆ瀽menuPath鑾峰彇鐩爣鑿滃崟
    QMenu* targetMenu = resolveMenu(desc.menuPath);
    if (!targetMenu) {
        std::cerr << "[MenuBuilder] Cannot resolve menuPath: " << desc.menuPath << "\n";
        return;
    }

    // 閫氳繃 UIActionBridge 鍒涘缓 QAction
    QAction* action = gBridge->createQAction(desc, targetMenu);
    actionMap_[desc.id] = action;

    // 瑙ｆ瀽鎻掑叆浣嶇疆
    auto info = MenuDescriptor::parse(desc.menuPath);
    insertIntoMenu(targetMenu, action, info.insertPosition);
}

QMenu* MenuBuilder::resolveMenu(const std::string& menuPath) {
    // 鍏堟鏌ョ紦瀛?    auto it = menuPathCache_.find(menuPath);
    if (it != menuPathCache_.end()) {
        return it->second;
    }

    // 瑙ｆ瀽璺緞
    QList<QMenu*> menus = resolveMenuPath(menuPath);
    if (menus.isEmpty()) {
        return nullptr;
    }

    // 杩斿洖鏈€鍚庝竴绾ц彍鍗?    QMenu* result = menus.last();
    menuPathCache_[menuPath] = result;
    return result;
}

QList<QMenu*> MenuBuilder::resolveMenuPath(const std::string& menuPath) {
    QList<QMenu*> result;
    
    // 瑙ｆ瀽璺緞娈?    std::vector<std::string> segments;
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
        // 鎺掗櫎鏈熬鐨?before:/after:/--- 绛夋爣璁?        if (current.find("before:") != 0 && 
            current.find("after:") != 0 &&
            current != "---") {
            segments.push_back(current);
        }
    }

    if (segments.empty()) {
        return result;
    }

    // 浠庨《灞傝彍鍗曞紑濮?    QString topKey = QString::fromStdString(segments[0]).toLower();
    QMenu* currentMenu = nullptr;

    // 鏌ユ壘椤跺眰鑿滃崟
    auto topIt = topMenus_.find(topKey.toStdString());
    if (topIt != topMenus_.end()) {
        currentMenu = topIt->second;
        result.append(currentMenu);
    } else {
        // 鍔ㄦ€佸垱寤烘柊鐨勯《灞傝彍鍗?        currentMenu = menuBar_->addMenu(QString::fromStdString(segments[0]));
        topMenus_[segments[0]] = currentMenu;
        topMenus_[topKey.toStdString()] = currentMenu;
        result.append(currentMenu);
    }

    // 閫愮骇鏌ユ壘鎴栧垱寤哄瓙鑿滃崟
    for (size_t i = 1; i < segments.size(); ++i) {
        QString segment = QString::fromStdString(segments[i]);
        
        // 鏌ユ壘鐜版湁瀛愯彍鍗?        QMenu* subMenu = nullptr;
        for (QAction* a : currentMenu->actions()) {
            if (a->menu() && a->text() == segment) {
                subMenu = a->menu();
                break;
            }
        }

        // 濡傛灉涓嶅瓨鍦紝鍒涘缓鏂扮殑瀛愯彍鍗?        if (!subMenu) {
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

    // 鎵句笉鍒板弬鑰冪偣鍒欒拷鍔犲埌鏈熬
    menu->addAction(action);
}

QMenu* MenuBuilder::getTopMenu(const std::string& displayName) const {
    auto it = topMenus_.find(displayName);
    return (it != topMenus_.end()) ? it->second : nullptr;
}

#endif // CAE_ENABLE_GUI
