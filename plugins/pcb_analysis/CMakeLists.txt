cmake_minimum_required(VERSION 3.16)
include(${CMAKE_SOURCE_DIR}/cmake/PluginHelpers.cmake)

# ============================================================================
# pcb_analysis 插件 - HYBRID 类型
# ============================================================================
# 定义插件导出宏（必须在 add_library 之前）
add_definitions(-DCAE_PLUGIN_PCB_ANALYSIS)

add_cae_plugin(cae_plugin_pcb_analysis
    SOURCES
        src/PCBAnalysisPlugin.cpp
    TYPE HYBRID
)

target_include_directories(cae_plugin_pcb_analysis
    PRIVATE
        ${CMAKE_SOURCE_DIR}/core/include
)

# HYBRID插件：GUI模式下链接UI库
if(ENABLE_GUI)
    target_link_libraries(cae_plugin_pcb_analysis PRIVATE cae_ui)
    target_compile_definitions(cae_plugin_pcb_analysis PRIVATE CAE_ENABLE_GUI)
endif()
