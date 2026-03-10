#pragma once
#include <string>
#include <vector>

/// @brief 鑿滃崟鑺傜偣淇℃伅锛堣В鏋恗enuPath鍚庣殑缁撴灉锛?
struct MenuInsertInfo {
    std::vector<std::string> menuHierarchy;  ///< 鑿滃崟灞傜骇锛屽 ["Desktop", "File"]
    std::string insertPosition;              ///< 鎻掑叆浣嶇疆锛?"=鏈熬 / "before:id" / "after:id"
    bool isSeparator{false};                 ///< 鏄惁涓哄垎闅旂鑺傜偣
};

/// @brief 鑿滃崟璺緞瑙ｆ瀽宸ュ叿
/// 灏?menuPath 瀛楃涓茶В鏋愪负缁撴瀯鍖栫殑鎻掑叆淇℃伅
class MenuDescriptor {
public:
    /// @brief 瑙ｆ瀽 menuPath 瀛楃涓?
    /// 鏍煎紡绀轰緥锛?
    ///   "desktop_menu"                           鈫?Desktop鑿滃崟鏈熬
    ///   "desktop_menu/file_group"                鈫?Desktop/File缁勬湯灏?
    ///   "view_menu/before:zoom_out"              鈫?View鑿滃崟涓瓃oom_out鍓?
    ///   "---"                                    鈫?鍒嗛殧绗?
    static MenuInsertInfo parse(const std::string& menuPath);

    /// @brief 灏唌enu_path椋庢牸鐨勫悕绉拌浆鎹负鏄剧ず鍚嶇О
    /// 濡?"desktop_menu" 鈫?"Desktop", "file_group" 鈫?"File"
    static std::string toDisplayName(const std::string& pathSegment);
};
