cmake_minimum_required(VERSION 3.16)

if(NOT ENABLE_PYTHON)
    return()
endif()

find_package(pybind11 REQUIRED)

pybind11_add_module(cae_plugin
    src/PythonAdapter.cpp
    src/PythonModule.cpp
)

target_include_directories(cae_plugin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/core/include
)

target_link_libraries(cae_plugin PRIVATE cae_core)

target_compile_features(cae_plugin PRIVATE cxx_std_17)
target_compile_definitions(cae_plugin PRIVATE CAE_ENABLE_PYTHON)

# 输出到 build/python/ 目录，方便直接 import
set_target_properties(cae_plugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/python
)

install(TARGETS cae_plugin LIBRARY DESTINATION python)
