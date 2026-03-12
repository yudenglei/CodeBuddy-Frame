#pragma once
#include <string>
#include <vector>

// 前向声明
class ActionContext;

/// @brief Action分发器（兼容层）
///
/// 功能：
/// - 提供统一的Action分发接口
/// - 委托给ActionManager::invoke
/// - 保留此类用于向后兼容
///
/// 新代码建议直接使用 ActionManager::instance().invoke()
class ActionDispatcher {
public:
    /// @brief 获取单例实例
    static ActionDispatcher& instance();

    /// @brief 触发单个Action
    /// @param actionId Action唯一标识
    /// @param ctx 执行上下文（可选）
    void dispatch(const std::string& actionId, const ActionContext& ctx = {});

    /// @brief 批量触发Action序列
    /// @param ids Action标识列表
    /// @param ctx 执行上下文（可选）
    void dispatchSequence(const std::vector<std::string>& ids, const ActionContext& ctx = {});

private:
    ActionDispatcher() = default;
    ~ActionDispatcher() = default;

    ActionDispatcher(const ActionDispatcher&) = delete;
    ActionDispatcher& operator=(const ActionDispatcher&) = delete;
};