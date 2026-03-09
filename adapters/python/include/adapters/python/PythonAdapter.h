#pragma once
#include "core/ActionManager.h"
#include "core/ActionContext.h"
#include <string>
#include <vector>
#include <functional>

/// @brief Python适配器接口（pybind11封装前的C++包装层）
///
/// 提供 Python 友好的接口，最终委托给 ActionManager 单例
class PythonAdapter {
public:
    static PythonAdapter& instance();

    /// @brief 触发Action（与GUI点击完全等价）
    void invoke(const std::string& actionId,
                const std::unordered_map<std::string, std::string>& params = {});

    /// @brief 注册自定义Action（Python lambda回调）
    void registerAction(const std::string& id,
                        const std::string& label,
                        std::function<void()> callback,
                        const std::string& menuPath = "");

    /// @brief 获取所有已注册Action信息
    std::vector<ActionDescriptor> listActions() const;

    /// @brief 更新快捷键
    void setShortcut(const std::string& id, const std::string& shortcut);

private:
    PythonAdapter() = default;
};
