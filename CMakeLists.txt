cmake_minimum_required(VERSION 3.16)

project(CaePCBPlugin
    VERSION 1.0.0
    DESCRIPTION "CAE High-Speed PCB Plugin System (KLayout-inspired, HFSS 3D Layout style)"
    LANGUAGES CXX
)

# ==============================================================
# 鏋勫缓妯″紡寮€鍏筹紙缂栬瘧鏈熼€夋嫨鍔熻兘锛?
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
# CMake 妯″潡璺緞
# ==============================================================
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CompilerOptions)

# Qt5 璺緞鎻愮ず锛堝湪 find_package 涔嬪墠璁剧疆锛?
if(ENABLE_GUI)
    include(FindQt5)
endif()

# ==============================================================
# 鍏ㄥ眬缂栬瘧璁剧疆
# ==============================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 杈撳嚭鐩綍缁熶竴鍒?build/bin 鍜?build/lib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 瀵煎嚭 compile_commands.json锛堜緵 clangd/IDE 浣跨敤锛?
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ==============================================================
# 瀛愮洰褰?
# ==============================================================

# 1. core 鈥?闆跺閮ㄤ緷璧栵紝鎵€鏈夋ā寮忓叡浜?
add_subdirectory(core)

# 2. ui 鈥?Qt5渚濊禆锛屼粎 ENABLE_GUI 鏃剁紪璇?
if(ENABLE_GUI)
    add_subdirectory(ui)
endif()

# 3. adapters 鈥?Python/gRPC 鍙彃鎷旈€傞厤灞?
add_subdirectory(adapters)

# 4. plugins 鈥?鍐呯疆绀轰緥鎻掍欢
add_subdirectory(plugins)

# 5. app 鈥?鍚勬ā寮忓叆鍙ｅ彲鎵ц鏂囦欢
add_subdirectory(app)

# 6. tests 鈥?鍗曞厓娴嬭瘯
if(ENABLE_TESTS)
    add_subdirectory(tests)
endif()

# ==============================================================
# Install 閰嶇疆
# ==============================================================
include(GNUInstallDirs)
install(DIRECTORY ${CMAKE_BINARY_DIR}/plugins/
    DESTINATION plugins
    FILES_MATCHING PATTERN "*.dll" PATTERN "*.so"
)

# ==============================================================
# CPack 鎵撳寘閰嶇疆锛堝彲閫夛級
# ==============================================================
set(CPACK_PACKAGE_NAME    "CaePCBPlugin")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR  "CAE Corp")
include(CPack)
