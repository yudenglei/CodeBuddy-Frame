#pragma once

/// @brief 杩愯妯″紡鏋氫妇
/// 鎺у埗鎻掍欢鍒濆鍖栬涓哄拰鍔熻兘鍙敤鎬?
enum class RunMode {
    GUI,        ///< Qt5 GUI妯″紡锛堝畬鏁寸晫闈級
    HEADLESS,   ///< 鏃犵晫闈㈡壒澶勭悊妯″紡
    PYTHON,     ///< Python鐩磋皟妯″紡锛坧ybind11锛?
    GRPC        ///< gRPC杩滅▼璋冪敤妯″紡
};
