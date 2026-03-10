cmake_minimum_required(VERSION 3.16)

# ---- GUI 鍙墽琛屾枃浠?----
if(ENABLE_GUI)
    find_package(Qt5 REQUIRED COMPONENTS Widgets)

    add_executable(cae_gui gui/main_gui.cpp)
    target_link_libraries(cae_gui PRIVATE cae_ui cae_core Qt5::Widgets)
    target_compile_definitions(cae_gui PRIVATE CAE_ENABLE_GUI)
    target_compile_features(cae_gui PRIVATE cxx_std_17)

    # Windows: 浣跨敤 WinMain 鍏ュ彛锛圙UI搴旂敤涓嶆樉绀烘帶鍒跺彴绐楀彛锛?
    if(WIN32)
        set_target_properties(cae_gui PROPERTIES WIN32_EXECUTABLE TRUE)
    endif()

    install(TARGETS cae_gui RUNTIME DESTINATION bin)
endif()

# ---- Headless 鍙墽琛屾枃浠讹紙鏃犻渶Qt锛?---
add_executable(cae_headless headless/main_headless.cpp)
target_link_libraries(cae_headless PRIVATE cae_core)
target_compile_features(cae_headless PRIVATE cxx_std_17)
install(TARGETS cae_headless RUNTIME DESTINATION bin)

# ---- gRPC 鏈嶅姟鍣?----
if(ENABLE_GRPC)
    add_executable(cae_grpc_server grpc_server/main_grpc.cpp)
    target_link_libraries(cae_grpc_server PRIVATE cae_core cae_grpc)
    target_compile_definitions(cae_grpc_server PRIVATE CAE_ENABLE_GRPC)
    target_compile_features(cae_grpc_server PRIVATE cxx_std_17)
    install(TARGETS cae_grpc_server RUNTIME DESTINATION bin)
endif()
