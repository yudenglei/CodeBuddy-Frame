#include "core/ActionManager.h"
#include <iostream>

/// @brief ActionDispatcher — 兼容层，委托给ActionManager::invoke
/// 保留此类用于向后兼容，新代码请直接使用 ActionManager::instance().invoke()
class ActionDispatcher {
public:
    static ActionDispatcher& instance() {
        static ActionDispatcher inst;
        return inst;
    }

    /// @brief 触发Action（委托给ActionManager）
    void dispatch(const std::string& actionId, const ActionContext& ctx = {}) {
        try {
            ActionManager::instance().invoke(actionId, ctx);
        } catch (const std::exception& e) {
            std::cerr << "[ActionDispatcher] Dispatch failed: " << e.what() << "\n";
        }
    }

    /// @brief 批量触发Action序列
    void dispatchSequence(const std::vector<std::string>& ids, const ActionContext& ctx = {}) {
        for (const auto& id : ids) {
            dispatch(id, ctx);
        }
    }

private:
    ActionDispatcher() = default;
};
