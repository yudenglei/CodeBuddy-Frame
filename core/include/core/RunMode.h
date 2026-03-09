#pragma once

/// @brief 运行模式枚举
/// 控制插件初始化行为和功能可用性
enum class RunMode {
    GUI,        ///< Qt5 GUI模式（完整界面）
    HEADLESS,   ///< 无界面批处理模式
    PYTHON,     ///< Python直调模式（pybind11）
    GRPC        ///< gRPC远程调用模式
};
