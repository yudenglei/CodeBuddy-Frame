cmake_minimum_required(VERSION 3.16)

# core 静态库：零外部依赖，所有模式共享
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

# C++17标准，引入编译选项
target_compile_features(cae_core PUBLIC cxx_std_17)

# Windows需要链接动态加载库（dlopen等效API已内置）
if(WIN32)
    # LoadLibrary等API在Kernel32中，MSVC默认链接，MinGW需显式指定
    if(MINGW)
        target_link_libraries(cae_core PUBLIC -lkernel32)
    endif()
else()
    # Linux需要链接dl和filesystem库
    target_link_libraries(cae_core PUBLIC dl)
    # GCC < 9 需要显式链接stdc++fs
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
        target_link_libraries(cae_core PUBLIC stdc++fs)
    endif()
endif()

# 安装规则
install(TARGETS cae_core
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)
install(DIRECTORY include/core DESTINATION include)
