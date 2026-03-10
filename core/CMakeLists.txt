cmake_minimum_required(VERSION 3.16)

# core 闈欐€佸簱锛氶浂澶栭儴渚濊禆锛屾墍鏈夋ā寮忓叡浜?
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

# C++17鏍囧噯锛屽紩鍏ョ紪璇戦€夐」
target_compile_features(cae_core PUBLIC cxx_std_17)

# Windows闇€瑕侀摼鎺ュ姩鎬佸姞杞藉簱锛坉lopen绛夋晥API宸插唴缃級
if(WIN32)
    # LoadLibrary绛堿PI鍦↘ernel32涓紝MSVC榛樿閾炬帴锛孧inGW闇€鏄惧紡鎸囧畾
    if(MINGW)
        target_link_libraries(cae_core PUBLIC -lkernel32)
    endif()
else()
    # Linux闇€瑕侀摼鎺l鍜宖ilesystem搴?
    target_link_libraries(cae_core PUBLIC dl)
    # GCC < 9 闇€瑕佹樉寮忛摼鎺tdc++fs
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
        target_link_libraries(cae_core PUBLIC stdc++fs)
    endif()
endif()

# 瀹夎瑙勫垯
install(TARGETS cae_core
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)
install(DIRECTORY include/core DESTINATION include)
