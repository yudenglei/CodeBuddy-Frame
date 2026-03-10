cmake_minimum_required(VERSION 3.16)
include(${CMAKE_SOURCE_DIR}/cmake/PluginHelpers.cmake)

# ============================================================================
# pcb_analysis 鎻掍欢 - HYBRID 绫诲瀷
# ============================================================================
# 瀹氫箟鎻掍欢瀵煎嚭瀹忥紙蹇呴』鍦?add_library 涔嬪墠锛?add_definitions(-DCAE_PLUGIN_PCB_ANALYSIS)

add_cae_plugin(cae_plugin_pcb_analysis
    SOURCES
        src/PCBAnalysisPlugin.cpp
    TYPE HYBRID
)

target_include_directories(cae_plugin_pcb_analysis
    PRIVATE
        ${CMAKE_SOURCE_DIR}/core/include
)

# HYBRID鎻掍欢锛欸UI妯″紡涓嬮摼鎺I搴?if(ENABLE_GUI)
    target_link_libraries(cae_plugin_pcb_analysis PRIVATE cae_ui)
    target_compile_definitions(cae_plugin_pcb_analysis PRIVATE CAE_ENABLE_GUI)
endif()
