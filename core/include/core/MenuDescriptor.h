#pragma once
#include <string>
#include <vector>

/// @brief 菜单节点信息（解析menuPath后的结果）
struct MenuInsertInfo {
    std::vector<std::string> menuHierarchy;  ///< 菜单层级，如 ["Desktop", "File"]
    std::string insertPosition;              ///< 插入位置：""=末尾 / "before:id" / "after:id"
    bool isSeparator{false};                 ///< 是否为分隔符节点
};

/// @brief 菜单路径解析工具
/// 将 menuPath 字符串解析为结构化的插入信息
class MenuDescriptor {
public:
    /// @brief 解析 menuPath 字符串
    /// 格式示例：
    ///   "desktop_menu"                           → Desktop菜单末尾
    ///   "desktop_menu/file_group"                → Desktop/File组末尾
    ///   "view_menu/before:zoom_out"              → View菜单中zoom_out前
    ///   "---"                                    → 分隔符
    static MenuInsertInfo parse(const std::string& menuPath);

    /// @brief 将menu_path风格的名称转换为显示名称
    /// 如 "desktop_menu" → "Desktop", "file_group" → "File"
    static std::string toDisplayName(const std::string& pathSegment);
};
