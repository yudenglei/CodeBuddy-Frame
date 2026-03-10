cmake_minimum_required(VERSION 3.16)

if(NOT ENABLE_GRPC)
    return()
endif()

include(${CMAKE_SOURCE_DIR}/cmake/FindgRPC.cmake)

# 生成 protobuf/gRPC 代码
set(PROTO_FILE ${CMAKE_CURRENT_SOURCE_DIR}/proto/plugin_service.proto)
set(GENERATED_DIR ${CMAKE_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GENERATED_DIR})

generate_grpc_cpp(${PROTO_FILE} ${GENERATED_DIR}
    SRCS GRPC_SRCS
    HDRS GRPC_HDRS
)

add_library(cae_grpc STATIC
    src/GrpcAdapter.cpp
    src/PluginServiceImpl.cpp
    src/GrpcClient.cpp
    ${GRPC_SRCS}
)

target_include_directories(cae_grpc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/core/include
    ${GENERATED_DIR}
)

target_link_libraries(cae_grpc PUBLIC
    cae_core
    gRPC::grpc++
    protobuf::libprotobuf
)

target_compile_definitions(cae_grpc PUBLIC CAE_ENABLE_GRPC)
target_compile_features(cae_grpc PUBLIC cxx_std_17)

install(TARGETS cae_grpc ARCHIVE DESTINATION lib)
