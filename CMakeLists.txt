cmake_minimum_required(VERSION 3.17)
project(cpp_demo)
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

add_executable(cpp_demo ${SOURCES} test/testRegex.cpp test/testSwitchNew.cpp src/vector2.cpp include/vector2.h test/testLambda.cpp test/constevalTest.cpp test/testTemplate.cpp test/testModernC++.cpp)
