cmake_minimum_required(VERSION 3.16)

project(CaePCBPlugin
    VERSION 1.0.0
    DESCRIPTION "CAE High-Speed PCB Plugin System (KLayout-inspired, HFSS 3D Layout style)"
    LANGUAGES CXX
)

# ==============================================================
# 构建模式开关（编译期选择功能）
# ==============================================================
option(ENABLE_GUI     "Build with Qt5 GUI support (Desktop/View/Layout/Tools menus)" ON)
option(ENABLE_PYTHON  "Build Python bindings via pybind11 (cae_plugin module)"      OFF)
option(ENABLE_GRPC    "Build gRPC server adapter"                                   OFF)
option(ENABLE_TESTS   "Build unit tests (requires GTest)"                           ON)

message(STATUS "=== CAE PCB Plugin System ===")
message(STATUS "  ENABLE_GUI    = ${ENABLE_GUI}")
message(STATUS "  ENABLE_PYTHON = ${ENABLE_PYTHON}")
message(STATUS "  ENABLE_GRPC   = ${ENABLE_GRPC}")
message(STATUS "  ENABLE_TESTS  = ${ENABLE_TESTS}")
message(STATUS "=============================")

# ==============================================================
# CMake 模块路径
# ==============================================================
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CompilerOptions)

# Qt5 路径提示（在 find_package 之前设置）
if(ENABLE_GUI)
    include(FindQt5)
endif()

# ==============================================================
# 全局编译设置
# ==============================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 输出目录统一到 build/bin 和 build/lib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 导出 compile_commands.json（供 clangd/IDE 使用）
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ==============================================================
# 子目录
# ==============================================================

# 1. core — 零外部依赖，所有模式共享
add_subdirectory(core)

# 2. ui — Qt5依赖，仅 ENABLE_GUI 时编译
if(ENABLE_GUI)
    add_subdirectory(ui)
endif()

# 3. adapters — Python/gRPC 可插拔适配层
add_subdirectory(adapters)

# 4. plugins — 内置示例插件
add_subdirectory(plugins)

# 5. app — 各模式入口可执行文件
add_subdirectory(app)

# 6. tests — 单元测试
if(ENABLE_TESTS)
    add_subdirectory(tests)
endif()

# ==============================================================
# Install 配置
# ==============================================================
include(GNUInstallDirs)
install(DIRECTORY ${CMAKE_BINARY_DIR}/plugins/
    DESTINATION plugins
    FILES_MATCHING PATTERN "*.dll" PATTERN "*.so"
)

# ==============================================================
# CPack 打包配置（可选）
# ==============================================================
set(CPACK_PACKAGE_NAME    "CaePCBPlugin")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR  "CAE Corp")
include(CPack)
