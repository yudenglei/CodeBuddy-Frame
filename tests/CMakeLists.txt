cmake_minimum_required(VERSION 3.16)

# 鏌ユ壘 GTest锛堜紭鍏堜娇鐢ㄧ郴缁熷簱锛屽け璐ユ椂鐢‵etchContent锛?find_package(GTest QUIET)

if(NOT GTest_FOUND)
    # 浣跨敤 FetchContent 涓嬭浇 GTest
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
    )
    # 璁剧疆涓轰笉浣跨敤绯荤粺鐨?GTest锛屽己鍒朵笅杞?    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

enable_testing()

add_executable(cae_tests
    test_action_manager.cpp
    test_dependency_sort.cpp
    test_plugin_manager.cpp
)

target_link_libraries(cae_tests
    PRIVATE
        cae_core
        GTest::gtest
        GTest::gtest_main
)

target_compile_features(cae_tests PRIVATE cxx_std_17)

# 娉ㄥ唽涓?CTest 娴嬭瘯
include(GoogleTest)
gtest_discover_tests(cae_tests)
