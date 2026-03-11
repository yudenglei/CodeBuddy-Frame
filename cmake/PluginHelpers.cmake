# cmake/PluginHelpers.cmake
# 提供 add_cae_plugin() 宏，统一插件编译规则
# ============================================================================
# 
# 使用方法：
#   add_cae_plugin(cae_plugin_myplugin
#       SOURCES src/MyPlugin.cpp
#       HEADERS include/MyPlugin.h
#       TYPE UI_ONLY|DB_ONLY|HYBRID
#   )
#
# 重要：
#   - 插件导出使用 CAE_PLUGIN_EXPORT 宏（在 PluginGlobal.h 中定义）
#   - 编译时自动定义 CAE_PLUGIN_EXPORTS 宏用于导出符号
#   - 链接 cae_core 提供核心接口
# ============================================================================

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
#   - 自动定义导出宏 CAE_PLUGIN_EXPORTS
#   - 生成 install 规则

macro(add_cae_plugin PLUGIN_NAME)
    # 解析参数
    cmake_parse_arguments(PLUGIN
        ""                  # 选项（无）
        "TYPE"              # 单值参数
        "SOURCES;HEADERS"   # 多值参数
        ${ARGN}
    )

    # 从插件名称提取宏名称（转换为大写）
    # 例如：cae_plugin_base_ui -> CAE_PLUGIN_BASE_UI
    string(TOUPPER "${PLUGIN_NAME}" PLUGIN_NAME_UPPER)
    string(REPLACE "CAE_PLUGIN_" "" PLUGIN_SUFFIX "${PLUGIN_NAME_UPPER}")
    set(PLUGIN_EXPORT_MACRO "CAE_PLUGIN_${PLUGIN_SUFFIX}")

    # 创建共享库目标
    add_library(${PLUGIN_NAME} SHARED ${PLUGIN_SOURCES} ${PLUGIN_HEADERS})

    # =========================================================================
    # 关键：定义导出宏
    # =========================================================================
    # CAE_PLUGIN_EXPORTS 用于 PluginGlobal.h 判断是导出还是导入
    # 同时定义插件专属宏供插件代码使用（如 CAE_PLUGIN_BASE_UI）
    target_compile_definitions(${PLUGIN_NAME} PRIVATE
        CAE_PLUGIN_EXPORTS
        ${PLUGIN_EXPORT_MACRO}
    )

    # 链接核心库
    target_link_libraries(${PLUGIN_NAME} PRIVATE cae_core)

    # 自动添加 core 头文件路径
    target_include_directories(${PLUGIN_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/core/include
    )

    # C++17
    target_compile_features(${PLUGIN_NAME} PRIVATE cxx_std_17)

    # 位置无关代码（Linux必需，Windows可选但推荐）
    set_target_properties(${PLUGIN_NAME} PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        # 输出到 build/plugins/ 目录（与 PluginManager::discover 默认路径一致）
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
        # 去掉 "lib" 前缀（Windows无影响，Linux下 .so 不加 lib 前缀）
        PREFIX ""
        # Windows: 生成导入库 (.lib)
        IMPORT_PREFIX ""
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

    message(STATUS "[PluginHelpers] Built plugin: ${PLUGIN_NAME}")
    message(STATUS "[PluginHelpers]   Export macro: ${PLUGIN_EXPORT_MACRO}")
endmacro()
