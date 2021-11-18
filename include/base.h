//
// Created by william on 2020/7/1.
//

#ifndef C___BASE_H
#define C___BASE_H

#include <algorithm>
#include <array>
#include <atomic>
#include <cassert>
#include <chrono>
#include <csignal>
#include <cstring>
#include <filesystem>
#include <forward_list>
#include <functional>
#include <future>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <random>
#include <regex>
#include <set>
#include <stack>
#include <string>
#include <string_view>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <valarray>
#include <variant>
#include <vector>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <glfw/deps/linmath.h>
#include <GLFW/glfw3.h>

/// assert macros
#define ASSERT(expression) assert(expression)

/// Error macro
#define LOG_ERROR(...)                                  \
    do                                                  \
    {                                                   \
        Logger::log(Logger::Level::Error, __VA_ARGS__); \
    } while (0)

/// Info macro
#define LOG_INFO(...)                                  \
    do                                                 \
    {                                                  \
        Logger::log(Logger::Level::Info, __VA_ARGS__); \
    } while (0)

/// Warn macro
#define LOG_WARN(...)                                  \
    do                                                 \
    {                                                  \
        Logger::log(Logger::Level::Warn, __VA_ARGS__); \
    } while (0)

#if defined(NDEBUG) && defined(__GNUC__)
    #define ASSERT_ONLY __attribute__((unused))
#else
    #define ASSERT_ONLY
#endif

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))

#define CGE_ENUM_ALIAS(name, member) \
    constexpr auto name##_##member = name::member

#include "logger.h"

#define GET_CURRENT(__VA_ARGS__) CURRENT_PROJECT_DIR #__VA_ARGS__

#define TO_STRING(s) #s
#define CONNECTENATE(x, y) x##y

#define NO_REF_TYPE(t) std::remove_reference<decltype(*t)>::type
#define MAKE_SHARED(t, ...) std::make_shared<typename NO_REF_TYPE(t)>(__VA_ARGS__)


/// math macros
#define MATH_PI 3.1415926f
#define TWO_MATH_PI (MATH_PI * 2.0f)
#define MATH_DEG_TO_RAD(x) ((x)*0.0174532925f)
#define MATH_PIOVER2 1.57079632679489661923f
#define MATH_EPSILON 0.000001f

#endif // C___BASE_H
