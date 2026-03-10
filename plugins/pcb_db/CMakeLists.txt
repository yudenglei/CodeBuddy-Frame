cmake_minimum_required(VERSION 3.16)
include(${CMAKE_SOURCE_DIR}/cmake/PluginHelpers.cmake)

# ============================================================================
# pcb_db 鎻掍欢 - DB_ONLY 绫诲瀷
# ============================================================================
# 瀹氫箟鎻掍欢瀵煎嚭瀹忥紙蹇呴』鍦?add_library 涔嬪墠锛?add_definitions(-DCAE_PLUGIN_PCB_DB)

add_cae_plugin(cae_plugin_pcb_db
    SOURCES
        src/PCBDBPlugin.cpp
    TYPE DB_ONLY
)

target_include_directories(cae_plugin_pcb_db
    PRIVATE
        ${CMAKE_SOURCE_DIR}/core/include
)
