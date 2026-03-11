cmake_minimum_required(VERSION 3.16)

# UI库：仅在 ENABLE_GUI=ON 时编译
if(NOT ENABLE_GUI)
    return()
endif()

find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_library(cae_ui STATIC
    src/MainWindow.cpp
    src/MenuBuilder.cpp
    src/UIActionBridge.cpp
    src/RibbonBar.cpp
)

target_include_directories(cae_ui
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)


target_link_libraries(cae_ui
    PUBLIC
        cae_core
        Qt5::Widgets
)

# GUI模式宏定义
target_compile_definitions(cae_ui PUBLIC CAE_ENABLE_GUI)

target_compile_features(cae_ui PUBLIC cxx_std_17)

install(TARGETS cae_ui ARCHIVE DESTINATION lib)
install(DIRECTORY include/ui DESTINATION include)
