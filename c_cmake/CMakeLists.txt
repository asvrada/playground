cmake_minimum_required(VERSION 3.16)
project(c_cmake)

set(CMAKE_CXX_STANDARD 17)

# # Add files under each folder
# list(APPEND SRCS_TRY_LINUX_API 
#     try_linux_api/mmap_related.h
# )

add_executable(c_cmake main.cpp)
