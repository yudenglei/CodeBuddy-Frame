cmake_minimum_required(VERSION 3.16)
include(${CMAKE_SOURCE_DIR}/cmake/PluginHelpers.cmake)

# ============================================================================
# base_ui 插件 - UI_ONLY 类型
# ============================================================================
# 定义插件导出宏（必须在 add_library 之前）
add_definitions(-DCAE_PLUGIN_BASE_UI)

add_cae_plugin(cae_plugin_base_ui
    SOURCES
        src/BaseUIPlugin.cpp
    HEADERS
        include/BaseUIPlugin.h
    TYPE UI_ONLY
)

target_include_directories(cae_plugin_base_ui
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/core/include
)

# GUI模式下链接 UI 库
if(ENABLE_GUI)
    target_link_libraries(cae_plugin_base_ui PRIVATE cae_ui)
    target_compile_definitions(cae_plugin_base_ui PRIVATE CAE_ENABLE_GUI)
endif()
