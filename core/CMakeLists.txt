cmake_minimum_required(VERSION 3.16)

add_library(cae_core STATIC
    src/ActionManager.cpp
    src/MenuDescriptor.cpp
    src/PluginManager.cpp
    src/DynamicLoader.cpp
    src/ActionDispatcher.cpp
)

target_include_directories(cae_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(cae_core PUBLIC cxx_std_17)

if(WIN32)
    if(MINGW)
        target_link_libraries(cae_core PUBLIC -lkernel32)
    endif()
else()
    target_link_libraries(cae_core PUBLIC dl)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
        target_link_libraries(cae_core PUBLIC stdc++fs)
    endif()
endif()

install(TARGETS cae_core ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
install(DIRECTORY include/core DESTINATION include)
