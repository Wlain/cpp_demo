cmake_minimum_required(VERSION 3.17)
project(cpp_demo)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

add_definitions(-DCURRENT_PROJECT_DIR="${PROJECT_SOURCE_DIR}")
find_package(OpenCV REQUIRED)

# 头文件目录
include_directories(
        ${OpenCV_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/graphics
        ${PROJECT_SOURCE_DIR}/3rdparty/lua-5.1.5/src
        ${PROJECT_SOURCE_DIR}/3rdparty/tolua-master/include
        ${PROJECT_SOURCE_DIR}/3rdparty/eigen3/include
)

file(GLOB_RECURSE SOURCES
        3rdparty/lua-5.1.5/src/*.c
        3rdparty/tolua-master/src/lib/*c
        include/*.h
        src/*.cpp
        graphics/*.cpp
        cpppreference/*.cpp
        test/*.cpp
        *.cpp
        )

add_executable(cpp_demo ${SOURCES} graphics/opencvTest.cpp graphics/eigen3Test.cpp)

set_target_properties(cpp_demo PROPERTIES COMPILE_FLAGS " -O0")

target_link_libraries(cpp_demo ${OpenCV_LIBS})