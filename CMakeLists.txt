cmake_minimum_required(VERSION 3.20)
project(cpp_demo)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

# 设置编译缓存
find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
    message(STATUS "Set up ccache ...")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    message(STATUS "found CCache: ${CCACHE_PROGRAM}")
endif ()

add_compile_options(-fno-elide-constructors)    #关闭编译器优化
set(CMAKE_CXX_FLAGS "-fno-elide-constructors ${CMAKE_CXX_FLAGS}")

add_definitions(-DCURRENT_PROJECT_DIR="${PROJECT_SOURCE_DIR}" -DIMGUI_IMPL_OPENGL_LOADER_GL3W)

# opencv
find_package(OpenCV REQUIRED)
# vulkan
find_package(Vulkan REQUIRED FATAL_ERROR)
# glfw
add_subdirectory(3rdparty/glfw)
# glew
add_subdirectory(3rdparty/glew-2.2.0/build/cmake)
# spdlog
add_subdirectory(3rdparty/spdlog)
# assimp
add_subdirectory(3rdparty/assimp)

# qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# 头文件目录
include_directories(
        ${OpenCV_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/math
        ${PROJECT_SOURCE_DIR}/utils
        ${PROJECT_SOURCE_DIR}/test
        ${PROJECT_SOURCE_DIR}/rasterizer
        ${PROJECT_SOURCE_DIR}/cg
        ${PROJECT_SOURCE_DIR}/graphic
        ${PROJECT_SOURCE_DIR}/graphic/vk
        ${PROJECT_SOURCE_DIR}/graphic/vk/test
        ${PROJECT_SOURCE_DIR}/graphic/gl
        ${PROJECT_SOURCE_DIR}/graphic/handle
        ${PROJECT_SOURCE_DIR}/graphic/gl/learnOpenGL
        ${PROJECT_SOURCE_DIR}/graphic/gl/test
        ${PROJECT_SOURCE_DIR}/rayTracing
        ${PROJECT_SOURCE_DIR}/qt
        ${PROJECT_SOURCE_DIR}/imageProcess
        ${PROJECT_SOURCE_DIR}/3rdparty
        ${PROJECT_SOURCE_DIR}/3rdparty/glm
        ${PROJECT_SOURCE_DIR}/3rdparty/glew-2.2.0/include
        ${PROJECT_SOURCE_DIR}/3rdparty/lua-5.1.5/src
        ${PROJECT_SOURCE_DIR}/3rdparty/SOIL2
        ${PROJECT_SOURCE_DIR}/3rdparty/tolua-master/include
        ${PROJECT_SOURCE_DIR}/3rdparty/eigen3/include
        ${PROJECT_SOURCE_DIR}/3rdparty/ann/include
        ${PROJECT_SOURCE_DIR}/3rdparty/rapidjson/include
        ${PROJECT_SOURCE_DIR}/3rdparty/imgui
        ${PROJECT_SOURCE_DIR}/3rdparty/imgui/backends
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        3rdparty/lua-5.1.5/src/*.c
        3rdparty/tolua-master/src/lib/*c
        3rdparty/SOIL2/*.c
        3rdparty/ann/src/*.cpp
        src/*.cpp
        cg/*.cpp
        rasterizer/*.cpp
        cpppreference/*.cpp
        math/*.cpp
        test/*.cpp
        utils/*.cpp
        graphic/common/*.cpp
        graphic/gl/*.cpp
        graphic/metal/*.mm
        rayTracing/*.cpp
        imageProcess/*.cpp
        cpp/*.cpp
        qt/*cpp)

# imgui
file(GLOB IMAGE_UI_SRC
        3rdparty/imgui/backends/imgui_impl_glfw.*
        3rdparty/imgui/backends/imgui_impl_opengl3.*
        3rdparty/imgui/*.cpp
        )

add_executable(cpp_demo ${SOURCES} ${IMAGE_UI_SRC} test/setCallbackTest.cpp test/charTest.cpp)

set_target_properties(cpp_demo PROPERTIES
        COMPILE_FLAGS " -O0"
        MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/graphic/metal/Info.plist)

target_compile_options(cpp_demo PUBLIC "-fobjc-arc")

# 链接库
target_link_libraries(cpp_demo
        ${OpenCV_LIBS}
        glfw
        glew
        spdlog
        assimp
        #        Vulkan::Vulkan
        "-framework IOSurface"
        "-framework Metal"
        "-framework MetalKit"
        "-framework QuartzCore"
        "-framework CoreMedia"
        "-framework AVFoundation"
        "-framework Accelerate"
        Qt5::Widgets
        Qt5::Gui
        Qt5::Core
        z
        )