cmake_minimum_required(VERSION 3.16)

if(ENABLE_PYTHON)
    add_subdirectory(python)
endif()

if(ENABLE_GRPC)
    add_subdirectory(grpc)
endif()
