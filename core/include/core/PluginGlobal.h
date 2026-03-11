#pragma once

// ============================================================================
// PluginGlobal.h - 跨平台插件导出宏
// ============================================================================
// 
// 使用方法：
// 1. 在插件的 CMakeLists.txt 中定义插件专属宏，如：add_definitions(-DCAE_PLUGIN_BASE_UI)
// 2. 在插件源文件中包含此头文件，并在导出函数前使用 CAE_PLUGIN_EXPORT
//
// 示例 CMakeLists.txt:
//   add_definitions(-DCAE_PLUGIN_BASE_UI)
//   target_compile_definitions(cae_plugin_base_ui PRIVATE CAE_PLUGIN_BASE_UI)
// ============================================================================

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    // Windows (MSVC or MinGW)
    #if defined(CAE_PLUGIN_EXPORTS)
        // 构建DLL时导出符号
        #define CAE_PLUGIN_EXPORT __declspec(dllexport)
    #else
        // 使用DLL时导入符号
        #define CAE_PLUGIN_EXPORT __declspec(dllimport)
    #endif
#else
    // Linux / macOS / Unix
    #if defined(__GNUC__) && __GNUC__ >= 4
        // GCC 4.0+ 支持 visibility 属性
        #define CAE_PLUGIN_EXPORT __attribute__((visibility("default")))
    #else
        #define CAE_PLUGIN_EXPORT
    #endif
#endif

// 便捷宏：用于显式禁用拷贝
#define CAE_PLUGIN_DISABLE_COPY(ClassName) \
    ClassName(const ClassName&) = delete; \
    ClassName& operator=(const ClassName&) = delete;

// 便捷宏：用于显式禁用移动
#define CAE_PLUGIN_DISABLE_MOVE(ClassName) \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(ClassName&&) = delete;
