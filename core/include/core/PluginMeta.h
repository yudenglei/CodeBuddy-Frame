#pragma once
#include <string>
#include <vector>

/// @brief 插件类型枚举
enum class PluginType {
    UI_ONLY,  ///< 纯UI插件，仅GUI模式加载
    DB_ONLY,  ///< 纯数据库/逻辑插件，所有模式可用
    HYBRID    ///< 混合型：逻辑层全模式可用，UI层仅GUI模式
};

/// @brief 插件元信息结构体
struct PluginMeta {
    std::string name;                          ///< 插件唯一名称（如 "pcb_analysis"）
    std::string version;                       ///< 版本号（如 "1.0.0"）
    std::string description;                   ///< 插件描述
    PluginType  type{PluginType::DB_ONLY};     ///< 插件类型
    std::vector<std::string> dependencies;     ///< 依赖的插件名称列表（Kahn排序用）
};
