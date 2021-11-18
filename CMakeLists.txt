cmake_minimum_required(VERSION 3.17)
project(cpp_demo)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

add_compile_options(-fno-elide-constructors)    #关闭编译器优化
set(CMAKE_CXX_FLAGS "-fno-elide-constructors ${CMAKE_CXX_FLAGS}")

add_definitions(-DCURRENT_PROJECT_DIR="${PROJECT_SOURCE_DIR}")

# opencv
find_package(OpenCV REQUIRED)
# vulkan
find_package(Vulkan REQUIRED FATAL_ERROR)
# glfw
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glfw)
# glew
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glew/build/cmake)
# glm
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glm/glm)

# 头文件目录
include_directories(
        ${OpenCV_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/math
        ${PROJECT_SOURCE_DIR}/utils
        ${PROJECT_SOURCE_DIR}/rasterizer
        ${PROJECT_SOURCE_DIR}/graphic
        ${PROJECT_SOURCE_DIR}/graphic/vk
        ${PROJECT_SOURCE_DIR}/graphic/vk/test
        ${PROJECT_SOURCE_DIR}/graphic/gl
        ${PROJECT_SOURCE_DIR}/graphic/gl/common
        ${PROJECT_SOURCE_DIR}/graphic/gl/test
        ${PROJECT_SOURCE_DIR}/3rdparty
        ${PROJECT_SOURCE_DIR}/3rdparty/lua-5.1.5/src
        ${PROJECT_SOURCE_DIR}/3rdparty/SOIL2
        ${PROJECT_SOURCE_DIR}/3rdparty/tolua-master/include
        ${PROJECT_SOURCE_DIR}/3rdparty/eigen3/include)

file(GLOB_RECURSE SOURCES
        3rdparty/lua-5.1.5/src/*.c
        3rdparty/tolua-master/src/lib/*c
        3rdparty/SOIL2/*.c
        include/*.h
        src/*.cpp
        rasterizer/*.cpp
        cpppreference/*.cpp
        math/*.cpp
        test/*.cpp
        utils/*.cpp
        graphic/*.cpp)

add_executable(cpp_demo ${SOURCES} )

set_target_properties(cpp_demo PROPERTIES COMPILE_FLAGS " -O0")

# 链接库
target_link_libraries(cpp_demo
        ${OpenCV_LIBS}
        glfw
        glew
        Vulkan::Vulkan)

