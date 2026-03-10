#pragma once
#include <string>
#include <unordered_map>
#include <variant>
#include "RunMode.h"

/// @brief Action鎵ц涓婁笅鏂?
/// 灏佽璋冪敤鏉ユ簮淇℃伅鍜屽弬鏁板寘
struct ActionContext {
    /// 璋冪敤鏉ユ簮妯″紡锛堢敤浜嶢ction鍐呴儴宸紓鍖栧鐞嗭級
    RunMode sourceMode{RunMode::HEADLESS};

    /// 閫氱敤鍙傛暟鍖咃細鏀寔 string / int / double / bool 鍥涚绫诲瀷
    using ParamValue = std::variant<std::string, int, double, bool>;
    std::unordered_map<std::string, ParamValue> params;

    /// 渚挎嵎鍙傛暟鑾峰彇锛堢被鍨嬪畨鍏級
    template<typename T>
    T get(const std::string& key, const T& defaultVal = T{}) const {
        auto it = params.find(key);
        if (it == params.end()) return defaultVal;
        const T* val = std::get_if<T>(&it->second);
        return val ? *val : defaultVal;
    }
};
