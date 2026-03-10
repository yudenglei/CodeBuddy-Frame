# cmake/CompilerOptions.cmake
# 闆嗕腑绠＄悊 C++17 鏍囧噯銆佽鍛婄骇鍒€佽法骞冲彴棰勫鐞嗗畯

function(apply_cae_compiler_options target)
    # C++17 鏍囧噯
    target_compile_features(${target} PUBLIC cxx_std_17)

    # 璺ㄥ钩鍙板畯瀹氫箟
    if(WIN32)
        target_compile_definitions(${target} PRIVATE
            WIN32_LEAN_AND_MEAN     # 鍑忓皯 windows.h 鍖呭惈浣撶Н
            NOMINMAX                # 绂佺敤 min/max 瀹忥紙涓?std::min/max 鍐茬獊锛?
            _CRT_SECURE_NO_WARNINGS # 绂佺敤 MSVC 瀹夊叏璀﹀憡
            _WIN32_WINNT=0x0601     # 鏈€浣?Windows 7
        )
    endif()

    # 璀﹀憡绾у埆
    if(MSVC)
        target_compile_options(${target} PRIVATE
            /W4         # 楂樿鍛婄骇鍒?
            /WX-        # 璀﹀憡涓嶄綔涓洪敊璇紙鍙寜闇€鏀逛负 /WX锛?
            /permissive-# 涓ユ牸鏍囧噯妯″紡
            /utf-8      # UTF-8 婧愭枃浠?
        )
    else()
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wno-unused-parameter
        )
    endif()

    # Linux 闇€瑕?PIC锛堜綅缃棤鍏充唬鐮侊級鐢ㄤ簬鍔ㄦ€佸簱
    if(UNIX)
        set_target_properties(${target} PROPERTIES POSITION_INDEPENDENT_CODE ON)
    endif()
endfunction()
