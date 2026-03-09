#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "ActionContext.h"

/// @brief Action描述符
/// 注册时携带所有元信息，包括菜单插入位置
struct ActionDescriptor {
    std::string id;           ///< 唯一ID，如 "Desktop.Save"
    std::string label;        ///< 菜单/按钮显示文字
    std::string tooltip;      ///< 工具提示
    std::string iconPath;     ///< 图标资源路径（可选）
    std::string shortcut;     ///< 快捷键，如 "Ctrl+S"
    /// 菜单插入路径，格式：
    ///   "desktop_menu/file_group"          - 插入到file_group末尾
    ///   "desktop_menu/file_group/before:save_action" - 在save_action前插入
    ///   "---"                              - 分隔符节点
    std::string menuPath;
    std::function<void(const ActionContext&)> callback; ///< 执行回调
};

/// @brief ActionManager — 统一Action注册与调用枢纽（单例，参考KLayout）
///
/// 三条调用路径均通过 invoke(id) 统一触发：
///   GUI菜单点击  → UIActionBridge → invoke()
///   Python脚本   → am.invoke("id")
///   gRPC请求     → PluginServiceImpl → invoke()
class ActionManager {
public:
    /// @brief 获取全局单例
    static ActionManager& instance();

    // 禁止拷贝/移动
    ActionManager(const ActionManager&) = delete;
    ActionManager& operator=(const ActionManager&) = delete;

    /// @brief 注册Action（重复ID则覆盖，并通知所有Observer）
    void registerAction(const ActionDescriptor& desc);

    /// @brief 触发Action（O(1)查找）
    /// @throws std::runtime_error 若id不存在
    void invoke(const std::string& id, const ActionContext& ctx = {});

    /// @brief 更新快捷键
    void setShortcut(const std::string& id, const std::string& shortcut);

    /// @brief 查询单个Action描述符（找不到返回nullptr）
    const ActionDescriptor* findAction(const std::string& id) const;

    /// @brief 获取所有已注册Action列表（按注册顺序）
    std::vector<ActionDescriptor> listActions() const;

    /// @brief 注册Observer：当有新Action注册时回调（UI层用于自动构建菜单）
    void onActionRegistered(std::function<void(const ActionDescriptor&)> cb);

    /// @brief 清空所有注册（测试用）
    void clear();

private:
    ActionManager() = default;

    std::unordered_map<std::string, ActionDescriptor> actions_;
    std::vector<std::string> registrationOrder_;   ///< 保持注册顺序
    std::vector<std::function<void(const ActionDescriptor&)>> observers_;
};
