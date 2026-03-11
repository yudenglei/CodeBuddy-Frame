cmake_minimum_required(VERSION 3.16)
include(${CMAKE_SOURCE_DIR}/cmake/PluginHelpers.cmake)

# ============================================================================
# pcb_db 插件 - DB_ONLY 类型
# ============================================================================
# 定义插件导出宏（必须在 add_library 之前）
add_definitions(-DCAE_PLUGIN_PCB_DB)

add_cae_plugin(cae_plugin_pcb_db
    SOURCES
        src/PCBDBPlugin.cpp
    TYPE DB_ONLY
)

target_include_directories(cae_plugin_pcb_db
    PRIVATE
        ${CMAKE_SOURCE_DIR}/core/include
)
