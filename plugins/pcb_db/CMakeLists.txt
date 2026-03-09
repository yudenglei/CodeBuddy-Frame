cmake_minimum_required(VERSION 3.16)
include(${CMAKE_SOURCE_DIR}/cmake/PluginHelpers.cmake)
add_cae_plugin(cae_plugin_pcb_db SOURCES src/PCBDBPlugin.cpp TYPE DB_ONLY)
