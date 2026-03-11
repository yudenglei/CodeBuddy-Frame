#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "ActionContext.h"

// ============================================================================
// RibbonInfo - 工具栏（Ribbon）信息
// ============================================================================

/// @brief 工具栏按钮大小类型
enum class ToolButtonSize {
    Large,      ///< 大按钮：图标在上，文字在下（32x32图标）
    Small,      ///< 小按钮：图标在左，文字在右（16x16图标）
    IconOnly    ///< 仅图标：无文字
};

/// @brief 工具栏/Ribbon信息
/// 插件通过此结构注册工具栏按钮
struct RibbonInfo {
    std::string tabId;           ///< 所属Tab ID（如 "Draw", "Modeler", "HFSS"）
    std::string tabTitle;        ///< Tab显示标题（如 "绘制", "建模"）
    std::string groupId;         ///< 所属Group ID（如 "Primitives", "Boolean"）
    std::string groupTitle;      ///< Group显示标题（如 "基本体", "布尔运算"）
    ToolButtonSize size{ToolButtonSize::Large}; ///< 按钮大小
    int tabOrder{0};            ///< Tab排序（越小越靠前）
    int groupOrder{0};          ///< Group排序（越小越靠前）
    int itemOrder{0};           ///< 按钮在Group内排序
    bool isSplitButton{false};  ///< 是否为分裂按钮（下拉菜单）
    std::vector<std::string> splitItems; ///< 分裂按钮的子项ID列表
};

/// @brief Action描述符（扩展版）
/// 注册时携带所有元信息，包括菜单插入位置和工具栏配置
struct ActionDescriptor {
    std::string id;
    std::string label;
    std::string tooltip;
    std::string iconPath;
    std::string shortcut;
    std::string menuPath;
    RibbonInfo ribbon;
    std::function<void(const ActionContext&)> callback;
    
    ActionDescriptor() = default;
};

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
