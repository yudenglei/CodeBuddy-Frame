#pragma once
#include <string>
#include "ActionContext.h"

/// @brief 统一Action接口（Command Pattern）
/// GUI菜单、Python invoke()、gRPC ExecuteAction 均调用此接口
class IAction {
public:
    virtual ~IAction() = default;

    /// @brief 获取Action唯一ID（如 "Desktop.Save"）
    virtual std::string getId() const = 0;

    /// @brief 获取显示标签
    virtual std::string getLabel() const = 0;

    /// @brief 获取工具提示
    virtual std::string getTooltip() const { return ""; }

    /// @brief 执行Action（三条路径统一入口）
    virtual void execute(const ActionContext& ctx) = 0;

    /// @brief 当前是否可用（用于菜单灰化）
    virtual bool isEnabled() const { return true; }

    /// @brief 当前是否处于选中状态（用于Toggle Action）
    virtual bool isChecked() const { return false; }
};
