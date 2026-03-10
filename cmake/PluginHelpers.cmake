# cmake/PluginHelpers.cmake
# 鎻愪緵 add_cae_plugin() 瀹忥紝缁熶竴鎻掍欢缂栬瘧瑙勫垯
# ============================================================================
# 
# 浣跨敤鏂规硶锛?#   add_cae_plugin(cae_plugin_myplugin
#       SOURCES src/MyPlugin.cpp
#       HEADERS include/MyPlugin.h
#       TYPE UI_ONLY|DB_ONLY|HYBRID
#   )
#
# 閲嶈锛?#   - 鎻掍欢瀵煎嚭浣跨敤 CAE_PLUGIN_EXPORT 瀹忥紙鍦?PluginGlobal.h 涓畾涔夛級
#   - 缂栬瘧鏃惰嚜鍔ㄥ畾涔?CAE_PLUGIN_EXPORTS 瀹忕敤浜庡鍑虹鍙?#   - 閾炬帴 cae_core 鎻愪緵鏍稿績鎺ュ彛
# ============================================================================

include_guard(GLOBAL)

# add_cae_plugin(<name>
#   SOURCES <src1> <src2> ...
#   [HEADERS <hdr1> ...]
#   [TYPE UI_ONLY|DB_ONLY|HYBRID]
# )
#
# 鍔熻兘锛?#   - 缂栬瘧涓哄叡浜簱锛?dll / .so锛?#   - 鑷姩璁剧疆浣嶇疆鏃犲叧浠ｇ爜锛?fPIC锛?#   - 閾炬帴 cae_core
#   - 璁剧疆杈撳嚭鐩綍鍒?${CMAKE_BINARY_DIR}/plugins/
#   - 鑷姩瀹氫箟瀵煎嚭瀹?CAE_PLUGIN_EXPORTS
#   - 鐢熸垚 install 瑙勫垯

macro(add_cae_plugin PLUGIN_NAME)
    # 瑙ｆ瀽鍙傛暟
    cmake_parse_arguments(PLUGIN
        ""                  # 閫夐」锛堟棤锛?        "TYPE"              # 鍗曞€煎弬鏁?        "SOURCES;HEADERS"   # 澶氬€煎弬鏁?        ${ARGN}
    )

    # 浠庢彃浠跺悕绉版彁鍙栧畯鍚嶇О锛堣浆鎹负澶у啓锛?    # 渚嬪锛歝ae_plugin_base_ui -> CAE_PLUGIN_BASE_UI
    string(TOUPPER "${PLUGIN_NAME}" PLUGIN_NAME_UPPER)
    string(REPLACE "CAE_PLUGIN_" "" PLUGIN_SUFFIX "${PLUGIN_NAME_UPPER}")
    set(PLUGIN_EXPORT_MACRO "CAE_PLUGIN_${PLUGIN_SUFFIX}")

    # 鍒涘缓鍏变韩搴撶洰鏍?    add_library(${PLUGIN_NAME} SHARED ${PLUGIN_SOURCES} ${PLUGIN_HEADERS})

    # =========================================================================
    # 鍏抽敭锛氬畾涔夊鍑哄畯
    # =========================================================================
    # CAE_PLUGIN_EXPORTS 鐢ㄤ簬 PluginGlobal.h 鍒ゆ柇鏄鍑鸿繕鏄鍏?    # 鍚屾椂瀹氫箟鎻掍欢涓撳睘瀹忎緵鎻掍欢浠ｇ爜浣跨敤锛堝 CAE_PLUGIN_BASE_UI锛?    target_compile_definitions(${PLUGIN_NAME} PRIVATE
        CAE_PLUGIN_EXPORTS
        ${PLUGIN_EXPORT_MACRO}
    )

    # 閾炬帴鏍稿績搴?    target_link_libraries(${PLUGIN_NAME} PRIVATE cae_core)

    # 鑷姩娣诲姞 core 澶存枃浠惰矾寰?    target_include_directories(${PLUGIN_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/core/include
    )

    # C++17
    target_compile_features(${PLUGIN_NAME} PRIVATE cxx_std_17)

    # 浣嶇疆鏃犲叧浠ｇ爜锛圠inux蹇呴渶锛學indows鍙€変絾鎺ㄨ崘锛?    set_target_properties(${PLUGIN_NAME} PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        # 杈撳嚭鍒?build/plugins/ 鐩綍锛堜笌 PluginManager::discover 榛樿璺緞涓€鑷达級
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
        # 鍘绘帀 "lib" 鍓嶇紑锛圵indows鏃犲奖鍝嶏紝Linux涓?.so 涓嶅姞 lib 鍓嶇紑锛?        PREFIX ""
        # Windows: 鐢熸垚瀵煎叆搴?(.lib)
        IMPORT_PREFIX ""
    )

    # 鎻掍欢绫诲瀷瀹忥紙鐢ㄤ簬鏉′欢缂栬瘧锛?    if(DEFINED PLUGIN_TYPE)
        target_compile_definitions(${PLUGIN_NAME} PRIVATE
            CAE_PLUGIN_TYPE_${PLUGIN_TYPE}=1
        )
    endif()

    # Install 瑙勫垯
    install(TARGETS ${PLUGIN_NAME}
        RUNTIME DESTINATION plugins
        LIBRARY DESTINATION plugins
    )

    message(STATUS "[PluginHelpers] Built plugin: ${PLUGIN_NAME}")
    message(STATUS "[PluginHelpers]   Export macro: ${PLUGIN_EXPORT_MACRO}")
endmacro()
