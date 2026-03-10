#include "core/ActionManager.h"
#include "core/ActionContext.h"
#include <iostream>
#include <vector>

/// @brief ActionDispatcher 鈥?鍏煎灞傦紝濮旀墭缁橝ctionManager::invoke
/// 淇濈暀姝ょ被鐢ㄤ簬鍚戝悗鍏煎锛屾柊浠ｇ爜璇风洿鎺ヤ娇鐢?ActionManager::instance().invoke()
class ActionDispatcher {
public:
    static ActionDispatcher& instance() {
        static ActionDispatcher inst;
        return inst;
    }


    /// @brief 瑙﹀彂Action锛堝鎵樼粰ActionManager锛?    void dispatch(const std::string& actionId, const ActionContext& ctx = {}) {
        try {
            ActionManager::instance().invoke(actionId, ctx);
        } catch (const std::exception& e) {
            std::cerr << "[ActionDispatcher] Dispatch failed: " << e.what() << "\n";
        }
    }


    /// @brief 鎵归噺瑙﹀彂Action搴忓垪
    void dispatchSequence(const std::vector<std::string>& ids, const ActionContext& ctx = {}) {
        for (const auto& id : ids) {
            dispatch(id, ctx);
        }
    }


private:
    ActionDispatcher() = default;
};
