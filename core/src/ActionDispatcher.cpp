#include "core/ActionManager.h"
#include <iostream>

/// @brief ActionDispatcher — 兼容层，委托给ActionManager::invoke
class ActionDispatcher {
public:
    static ActionDispatcher& instance() { static ActionDispatcher inst; return inst; }
    void dispatch(const std::string& actionId, const ActionContext& ctx = {}) {
        try { ActionManager::instance().invoke(actionId, ctx); }
        catch (const std::exception& e) { std::cerr << "[ActionDispatcher] " << e.what() << "\n"; }
    }
    void dispatchSequence(const std::vector<std::string>& ids, const ActionContext& ctx = {}) {
        for (const auto& id : ids) dispatch(id, ctx);
    }
private:
    ActionDispatcher() = default;
};
