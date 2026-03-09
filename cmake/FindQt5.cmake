# cmake/FindQt5.cmake
# Qt5 查找辅助：提示常见安装路径，设置 CMAKE_PREFIX_PATH

# 常见 Qt5 安装路径提示（Windows / Linux / macOS）
set(_QT5_HINTS
    # Windows 常见路径
    "C:/Qt/5.13.2/msvc2017_64"
    "C:/Qt/5.13.2/msvc2019_64"
    "C:/Qt/5.13.0/msvc2017_64"
    "C:/Qt/Qt5.13.2/5.13.2/msvc2019_64"
    # Linux 常见路径
    "/opt/Qt/5.13.2/gcc_64"
    "/usr/local/Qt-5.13.2"
    # macOS
    "$ENV{HOME}/Qt/5.13.2/clang_64"
)

# 允许用户通过环境变量或命令行覆盖
if(DEFINED ENV{Qt5_DIR})
    list(INSERT _QT5_HINTS 0 $ENV{Qt5_DIR})
endif()

if(DEFINED Qt5_ROOT)
    list(INSERT _QT5_HINTS 0 ${Qt5_ROOT})
endif()

# 将提示路径加入 CMAKE_PREFIX_PATH
foreach(_hint ${_QT5_HINTS})
    if(EXISTS "${_hint}")
        list(APPEND CMAKE_PREFIX_PATH "${_hint}")
    endif()
endforeach()

# 注意：实际 find_package(Qt5 ...) 在各子目录的 CMakeLists.txt 中调用
message(STATUS "[FindQt5] CMAKE_PREFIX_PATH updated with Qt5 hints")
message(STATUS "[FindQt5] To specify Qt5 location: -DQt5_ROOT=<path>")
