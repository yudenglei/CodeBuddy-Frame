#pragma once
#include <string>
#include <unordered_map>
#include <variant>
#include "RunMode.h"

/// @brief Action执行上下文
/// 封装调用来源信息和参数包
struct ActionContext {
    /// 调用来源模式（用于Action内部差异化处理）
    RunMode sourceMode{RunMode::HEADLESS};

    /// 通用参数包：支持 string / int / double / bool 四种类型
    using ParamValue = std::variant<std::string, int, double, bool>;
    std::unordered_map<std::string, ParamValue> params;

    /// 便捷参数获取（类型安全）
    template<typename T>
    T get(const std::string& key, const T& defaultVal = T{}) const {
        auto it = params.find(key);
        if (it == params.end()) return defaultVal;
        const T* val = std::get_if<T>(&it->second);
        return val ? *val : defaultVal;
    }
};
