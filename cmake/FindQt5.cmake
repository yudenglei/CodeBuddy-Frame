# cmake/FindQt5.cmake
# Qt5 鏌ユ壘杈呭姪锛氭彁绀哄父瑙佸畨瑁呰矾寰勶紝璁剧疆 CMAKE_PREFIX_PATH

# 甯歌 Qt5 瀹夎璺緞鎻愮ず锛圵indows / Linux / macOS锛?
set(_QT5_HINTS
    # Windows 甯歌璺緞
    "C:/Qt/5.13.2/msvc2017_64"
    "C:/Qt/5.13.2/msvc2019_64"
    "C:/Qt/5.13.0/msvc2017_64"
    "C:/Qt/Qt5.13.2/5.13.2/msvc2019_64"
    # Linux 甯歌璺緞
    "/opt/Qt/5.13.2/gcc_64"
    "/usr/local/Qt-5.13.2"
    # macOS
    "$ENV{HOME}/Qt/5.13.2/clang_64"
)

# 鍏佽鐢ㄦ埛閫氳繃鐜鍙橀噺鎴栧懡浠よ瑕嗙洊
if(DEFINED ENV{Qt5_DIR})
    list(INSERT _QT5_HINTS 0 $ENV{Qt5_DIR})
endif()

if(DEFINED Qt5_ROOT)
    list(INSERT _QT5_HINTS 0 ${Qt5_ROOT})
endif()

# 灏嗘彁绀鸿矾寰勫姞鍏?CMAKE_PREFIX_PATH
foreach(_hint ${_QT5_HINTS})
    if(EXISTS "${_hint}")
        list(APPEND CMAKE_PREFIX_PATH "${_hint}")
    endif()
endforeach()

# 娉ㄦ剰锛氬疄闄?find_package(Qt5 ...) 鍦ㄥ悇瀛愮洰褰曠殑 CMakeLists.txt 涓皟鐢?
message(STATUS "[FindQt5] CMAKE_PREFIX_PATH updated with Qt5 hints")
message(STATUS "[FindQt5] To specify Qt5 location: -DQt5_ROOT=<path>")
