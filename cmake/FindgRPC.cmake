# cmake/FindgRPC.cmake
# gRPC + protobuf 查找和 proto 代码生成封装

find_package(Protobuf QUIET)
find_package(gRPC QUIET)

# 如果找不到，尝试通过 pkg-config
if(NOT gRPC_FOUND)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(GRPC QUIET grpc++)
        pkg_check_modules(PROTOBUF QUIET protobuf)
    endif()
endif()

# 查找 grpc_cpp_plugin 代码生成工具
find_program(GRPC_CPP_PLUGIN grpc_cpp_plugin
    HINTS
        /usr/local/bin
        /usr/bin
        $ENV{GRPC_ROOT}/bin
)

if(GRPC_CPP_PLUGIN)
    message(STATUS "[FindgRPC] Found grpc_cpp_plugin: ${GRPC_CPP_PLUGIN}")
else()
    message(WARNING "[FindgRPC] grpc_cpp_plugin not found. gRPC code generation will be skipped.")
endif()

# ------------------------------------------------------------
# generate_grpc_cpp(<proto_file> <output_dir>
#   SRCS <var>   - 输出：生成的.cc文件列表
#   HDRS <var>   - 输出：生成的.h文件列表
# )
# ------------------------------------------------------------
function(generate_grpc_cpp PROTO_FILE OUTPUT_DIR)
    cmake_parse_arguments(GEN "" "" "SRCS;HDRS" ${ARGN})

    get_filename_component(PROTO_NAME ${PROTO_FILE} NAME_WE)
    get_filename_component(PROTO_DIR  ${PROTO_FILE} DIRECTORY)

    set(PROTO_SRCS  "${OUTPUT_DIR}/${PROTO_NAME}.pb.cc")
    set(PROTO_HDRS  "${OUTPUT_DIR}/${PROTO_NAME}.pb.h")
    set(GRPC_SRCS   "${OUTPUT_DIR}/${PROTO_NAME}.grpc.pb.cc")
    set(GRPC_HDRS   "${OUTPUT_DIR}/${PROTO_NAME}.grpc.pb.h")

    if(GRPC_CPP_PLUGIN AND Protobuf_PROTOC_EXECUTABLE)
        add_custom_command(
            OUTPUT  ${PROTO_SRCS} ${PROTO_HDRS} ${GRPC_SRCS} ${GRPC_HDRS}
            COMMAND ${Protobuf_PROTOC_EXECUTABLE}
                    --grpc_out=${OUTPUT_DIR}
                    --cpp_out=${OUTPUT_DIR}
                    --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN}
                    -I${PROTO_DIR}
                    ${PROTO_FILE}
            DEPENDS ${PROTO_FILE}
            COMMENT "Generating gRPC/protobuf code from ${PROTO_FILE}"
        )
    else()
        message(WARNING "[FindgRPC] Skipping code generation (protoc or grpc_cpp_plugin not found)")
        # 创建空文件以避免构建失败
        foreach(_f ${PROTO_SRCS} ${PROTO_HDRS} ${GRPC_SRCS} ${GRPC_HDRS})
            if(NOT EXISTS ${_f})
                file(WRITE ${_f} "// Auto-generated placeholder\n")
            endif()
        endforeach()
    endif()

    # 向父作用域返回结果
    if(DEFINED GEN_SRCS)
        set(${GEN_SRCS} "${PROTO_SRCS};${GRPC_SRCS}" PARENT_SCOPE)
    endif()
    if(DEFINED GEN_HDRS)
        set(${GEN_HDRS} "${PROTO_HDRS};${GRPC_HDRS}" PARENT_SCOPE)
    endif()
endfunction()
