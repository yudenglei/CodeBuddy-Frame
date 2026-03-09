# cmake/PluginHelpers.cmake
# 提供 add_cae_plugin() 宏，统一插件编译规则

include_guard(GLOBAL)

# add_cae_plugin(<name>
#   SOURCES <src1> <src2> ...
#   [HEADERS <hdr1> ...]
#   [TYPE UI_ONLY|DB_ONLY|HYBRID]
# )
#
# 功能：
#   - 编译为共享库（.dll / .so）
#   - 自动设置位置无关代码（-fPIC）
#   - 链接 cae_core
#   - 设置输出目录到 ${CMAKE_BINARY_DIR}/plugins/
#   - 生成 install 规则

macro(add_cae_plugin PLUGIN_NAME)
    # 解析参数
    cmake_parse_arguments(PLUGIN
        ""                  # 选项（无）
        "TYPE"              # 单值参数
        "SOURCES;HEADERS"   # 多值参数
        ${ARGN}
    )

    # 创建共享库目标
    add_library(${PLUGIN_NAME} SHARED ${PLUGIN_SOURCES} ${PLUGIN_HEADERS})

    # 链接核心库
    target_link_libraries(${PLUGIN_NAME} PRIVATE cae_core)

    # C++17
    target_compile_features(${PLUGIN_NAME} PRIVATE cxx_std_17)

    # 位置无关代码（Linux必需）
    set_target_properties(${PLUGIN_NAME} PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        # 输出到 build/plugins/ 目录（与 PluginManager::discover 默认路径一致）
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
        # 去掉 "lib" 前缀（Windows无影响，Linux下 .so 不加 lib 前缀）
        PREFIX ""
    )

    # 插件类型宏（用于条件编译）
    if(DEFINED PLUGIN_TYPE)
        target_compile_definitions(${PLUGIN_NAME} PRIVATE
            CAE_PLUGIN_TYPE_${PLUGIN_TYPE}=1
        )
    endif()

    # Install 规则
    install(TARGETS ${PLUGIN_NAME}
        RUNTIME DESTINATION plugins
        LIBRARY DESTINATION plugins
    )
endmacro()
