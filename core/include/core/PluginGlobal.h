#pragma once

// ============================================================================
// PluginGlobal.h - 璺ㄥ钩鍙版彃浠跺鍑哄畯
// ============================================================================
// 
// 浣跨敤鏂规硶锛?// 1. 鍦ㄦ彃浠剁殑 CMakeLists.txt 涓畾涔夋彃浠朵笓灞炲畯锛屽锛歛dd_definitions(-DCAE_PLUGIN_BASE_UI)
// 2. 鍦ㄦ彃浠舵簮鏂囦欢涓寘鍚澶存枃浠讹紝骞跺湪瀵煎嚭鍑芥暟鍓嶄娇鐢?CAE_PLUGIN_EXPORT
//
// 绀轰緥 CMakeLists.txt:
//   add_definitions(-DCAE_PLUGIN_BASE_UI)
//   target_compile_definitions(cae_plugin_base_ui PRIVATE CAE_PLUGIN_BASE_UI)
// ============================================================================

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    // Windows (MSVC or MinGW)
    #if defined(CAE_PLUGIN_EXPORTS)
        // 鏋勫缓DLL鏃跺鍑虹鍙?        #define CAE_PLUGIN_EXPORT __declspec(dllexport)
    #else
        // 浣跨敤DLL鏃跺鍏ョ鍙?        #define CAE_PLUGIN_EXPORT __declspec(dllimport)
    #endif
#else
    // Linux / macOS / Unix
    #if defined(__GNUC__) && __GNUC__ >= 4
        // GCC 4.0+ 鏀寔 visibility 灞炴€?        #define CAE_PLUGIN_EXPORT __attribute__((visibility("default")))
    #else
        #define CAE_PLUGIN_EXPORT
    #endif
#endif

// 渚挎嵎瀹忥細鐢ㄤ簬鏄惧紡绂佺敤鎷疯礉
#define CAE_PLUGIN_DISABLE_COPY(ClassName) \
    ClassName(const ClassName&) = delete; \
    ClassName& operator=(const ClassName&) = delete;

// 渚挎嵎瀹忥細鐢ㄤ簬鏄惧紡绂佺敤绉诲姩
#define CAE_PLUGIN_DISABLE_MOVE(ClassName) \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(ClassName&&) = delete;
