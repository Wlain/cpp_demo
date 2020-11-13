cmake_minimum_required(VERSION 3.16)
project(cpp_demo)
# 设置用debug还是release模式。debug允许断点，而release更快
set( CMAKE_BUILD_TYPE Debug )
# 允许c++11标准、O3优化、多线程。match选项可避免一些cpu上的问题
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -O3 -pthread" )
set(CMAKE_CXX_STANDARD 20)
# 头文件目录
include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

file(GLOB SOURCES
    3rdparty/lua-5.4.0/src/*.h
    3rdparty/lua-5.4.0/src/*.c
    include/*.h
    src/*.cpp
    test/*.cpp
    *.cpp
)

add_executable(cpp_demo ${SOURCES})
