cmake_minimum_required(VERSION 3.16)
include(${CMAKE_SOURCE_DIR}/cmake/PluginHelpers.cmake)

add_cae_plugin(cae_plugin_pcb_analysis
    SOURCES
        src/PCBAnalysisPlugin.cpp
    TYPE HYBRID
)

# HYBRID插件：GUI模式下链接UI库
if(ENABLE_GUI)
    target_link_libraries(cae_plugin_pcb_analysis PRIVATE cae_ui)
    target_compile_definitions(cae_plugin_pcb_analysis PRIVATE CAE_ENABLE_GUI)
endif()
