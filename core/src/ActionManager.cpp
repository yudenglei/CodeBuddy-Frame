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
    for (auto& obs : observers_) {
        obs(desc);
    }
}

void ActionManager::invoke(const std::string& id, const ActionContext& ctx) {
    auto it = actions_.find(id);
    if (it == actions_.end()) {
        throw std::runtime_error("ActionManager: unknown action id: " + id);
    }
    if (it->second.callback) {
        it->second.callback(ctx);
    }
}

void ActionManager::setShortcut(const std::string& id, const std::string& shortcut) {
    auto it = actions_.find(id);
    if (it != actions_.end()) {
        it->second.shortcut = shortcut;
        for (auto& obs : observers_) obs(it->second);
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
        if (it != actions_.end()) result.push_back(it->second);
    }
    return result;
}

void ActionManager::onActionRegistered(std::function<void(const ActionDescriptor&)> cb) {
    observers_.push_back(std::move(cb));
}

void ActionManager::clear() {
    actions_.clear();
    registrationOrder_.clear();
}
