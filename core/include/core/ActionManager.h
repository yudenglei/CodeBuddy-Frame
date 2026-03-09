#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "ActionContext.h"

/// @brief Action描述符
struct ActionDescriptor {
    std::string id;           ///< 唯一ID，如 "Desktop.Save"
    std::string label;        ///< 菜单/按钮显示文字
    std::string tooltip;
    std::string iconPath;
    std::string shortcut;     ///< 快捷键，如 "Ctrl+S"
    std::string menuPath;     ///< 菜单插入路径
    std::function<void(const ActionContext&)> callback;
};

/// @brief ActionManager — 统一Action注册与调用枢纽（单例，参考KLayout）
///
/// GUI菜单点击 → UIActionBridge → invoke()
/// Python脚本  → am.invoke("id")
/// gRPC请求    → PluginServiceImpl → invoke()
class ActionManager {
public:
    static ActionManager& instance();
    ActionManager(const ActionManager&) = delete;
    ActionManager& operator=(const ActionManager&) = delete;

    void registerAction(const ActionDescriptor& desc);
    void invoke(const std::string& id, const ActionContext& ctx = {});
    void setShortcut(const std::string& id, const std::string& shortcut);
    const ActionDescriptor* findAction(const std::string& id) const;
    std::vector<ActionDescriptor> listActions() const;
    void onActionRegistered(std::function<void(const ActionDescriptor&)> cb);
    void clear();

private:
    ActionManager() = default;
    std::unordered_map<std::string, ActionDescriptor> actions_;
    std::vector<std::string> registrationOrder_;
    std::vector<std::function<void(const ActionDescriptor&)>> observers_;
};
