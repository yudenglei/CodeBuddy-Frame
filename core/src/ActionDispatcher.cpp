#include "core/ActionDispatcher.h"
#include "core/ActionManager.h"
#include "core/ActionContext.h"
#include <iostream>

// ============================================================================
// 单例实例
// ============================================================================

ActionDispatcher& ActionDispatcher::instance() {
    static ActionDispatcher inst;
    return inst;
}

// ============================================================================
// Action分发
// ============================================================================

void ActionDispatcher::dispatch(const std::string& actionId, const ActionContext& ctx) {
    try {
        ActionManager::instance().invoke(actionId, ctx);
    } catch (const std::exception& e) {
        std::cerr << "[ActionDispatcher] Dispatch failed: " << e.what() << "\n";
    }
}

void ActionDispatcher::dispatchSequence(const std::vector<std::string>& ids, const ActionContext& ctx) {
    for (const auto& id : ids) {
        dispatch(id, ctx);
    }
}
