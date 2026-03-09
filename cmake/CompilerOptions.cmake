# cmake/CompilerOptions.cmake
# 集中管理 C++17 标准、警告级别、跨平台预处理宏

function(apply_cae_compiler_options target)
    # C++17 标准
    target_compile_features(${target} PUBLIC cxx_std_17)

    # 跨平台宏定义
    if(WIN32)
        target_compile_definitions(${target} PRIVATE
            WIN32_LEAN_AND_MEAN     # 减少 windows.h 包含体积
            NOMINMAX                # 禁用 min/max 宏（与 std::min/max 冲突）
            _CRT_SECURE_NO_WARNINGS # 禁用 MSVC 安全警告
            _WIN32_WINNT=0x0601     # 最低 Windows 7
        )
    endif()

    # 警告级别
    if(MSVC)
        target_compile_options(${target} PRIVATE
            /W4         # 高警告级别
            /WX-        # 警告不作为错误（可按需改为 /WX）
            /permissive-# 严格标准模式
            /utf-8      # UTF-8 源文件
        )
    else()
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wno-unused-parameter
        )
    endif()

    # Linux 需要 PIC（位置无关代码）用于动态库
    if(UNIX)
        set_target_properties(${target} PROPERTIES POSITION_INDEPENDENT_CODE ON)
    endif()
endfunction()
