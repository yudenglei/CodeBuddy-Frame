#include "core/ActionManager.h"
#include <stdexcept>
#include <algorithm>

ActionManager& ActionManager::instance() {
    static ActionManager inst;
    return inst;
}

void ActionManager::registerAction(const ActionDescriptor& desc) {
    bool isNew = (actions_.find(desc.id) == actions_.end());
    actions_[desc.id] = desc;

    if (isNew) {
        registrationOrder_.push_back(desc.id);
    }

    // 閫氱煡鎵€鏈塐bserver锛圲I灞傚皢鎹鑷姩鍒涘缓鎴栨洿鏂拌彍鍗曢」锛?
    for (auto& obs : observers_) {
        obs(desc);
    }
}

void ActionManager::invoke(const std::string& id, const ActionContext& ctx) {
    auto it = actions_.find(id);
    if (it == actions_.end()) {
        throw std::runtime_error("ActionManager: unknown action id: " + id);
    }
    const ActionDescriptor& desc = it->second;
    if (desc.callback) {
        desc.callback(ctx);
    }
}

void ActionManager::setShortcut(const std::string& id, const std::string& shortcut) {
    auto it = actions_.find(id);
    if (it != actions_.end()) {
        it->second.shortcut = shortcut;
        // 閫氱煡Observer鏇存柊蹇嵎閿?
        for (auto& obs : observers_) {
            obs(it->second);
        }
    }
}

const ActionDescriptor* ActionManager::findAction(const std::string& id) const {
    auto it = actions_.find(id);
    return (it != actions_.end()) ? &it->second : nullptr;
}

std::vector<ActionDescriptor> ActionManager::listActions() const {
    std::vector<ActionDescriptor> result;
    result.reserve(registrationOrder_.size());
    for (const auto& id : registrationOrder_) {
        auto it = actions_.find(id);
        if (it != actions_.end()) {
            result.push_back(it->second);
        }
    }
    return result;
}

void ActionManager::onActionRegistered(std::function<void(const ActionDescriptor&)> cb) {
    observers_.push_back(std::move(cb));
}

void ActionManager::clear() {
    actions_.clear();
    registrationOrder_.clear();
    // 淇濈暀observers锛屾祴璇曟椂鍙兘闇€瑕侀噸鏂版敞鍐?
}
