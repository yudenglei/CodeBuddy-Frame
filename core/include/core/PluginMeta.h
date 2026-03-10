#pragma once
#include <string>
#include <vector>

/// @brief 鎻掍欢绫诲瀷鏋氫妇
enum class PluginType {
    UI_ONLY,  ///< 绾疷I鎻掍欢锛屼粎GUI妯″紡鍔犺浇
    DB_ONLY,  ///< 绾暟鎹簱/閫昏緫鎻掍欢锛屾墍鏈夋ā寮忓彲鐢?
    HYBRID    ///< 娣峰悎鍨嬶細閫昏緫灞傚叏妯″紡鍙敤锛孶I灞備粎GUI妯″紡
};

/// @brief 鎻掍欢鍏冧俊鎭粨鏋勪綋
struct PluginMeta {
    std::string name;                          ///< 鎻掍欢鍞竴鍚嶇О锛堝 "pcb_analysis"锛?
    std::string version;                       ///< 鐗堟湰鍙凤紙濡?"1.0.0"锛?
    std::string description;                   ///< 鎻掍欢鎻忚堪
    PluginType  type{PluginType::DB_ONLY};     ///< 鎻掍欢绫诲瀷
    std::vector<std::string> dependencies;     ///< 渚濊禆鐨勬彃浠跺悕绉板垪琛紙Kahn鎺掑簭鐢級
};
