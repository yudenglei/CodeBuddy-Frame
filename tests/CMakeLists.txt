cmake_minimum_required(VERSION 3.16)

# 查找 GTest（优先使用 FetchContent，失败时用系统库）
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
    FIND_PACKAGE_ARGS NAMES GTest
)
FetchContent_MakeAvailable(googletest)

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

# 注册为 CTest 测试
include(GoogleTest)
gtest_discover_tests(cae_tests)
