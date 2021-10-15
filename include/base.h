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
#include <cstring>
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
#include <atomic>
#include <filesystem>
#include <csignal>

#define GET_CURRENT(__VA_ARGS__) CURRENT_PROJECT_DIR#__VA_ARGS__

#define TO_STRING( s ) #s
#define CONNECTENATE(x, y) x##y

#define NO_REF_TYPE(t) std::remove_reference<decltype(*t)>::type
#define MAKE_SHARED(t, ...) std::make_shared<typename NO_REF_TYPE(t)>(__VA_ARGS__)

#endif // C___BASE_H
