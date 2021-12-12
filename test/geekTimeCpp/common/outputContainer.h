//
// Created by william on 2021/12/11.
//

#ifndef CPP_DEMO_OUTPUTCONTAINER_H
#define CPP_DEMO_OUTPUTCONTAINER_H

#include <iostream>    // std::ostream
#include <type_traits> // std::false_type/true_type/decay_t/is_same_v
#include <utility>     // std::declval/pair

// Type trait to detect std::pair
template <typename T>
struct isPair : std::false_type
{};
template <typename T, typename U>
struct isPair<std::pair<T, U>> : std::false_type
{};
template <typename T>
inline constexpr bool isPairV = std::false_type{};

/// declval返回类型T的右值引用
template <typename T>
struct hasOutPutFunction
{
    template <class U>
    static auto output(U* ptr) -> decltype(std::declval<std::ostream&>() << *ptr, std::true_type());
    template <class U>
    static std::false_type output(...);
    static constexpr bool value = decltype(output<T>(nullptr))::value;
};

template <typename T>
inline constexpr bool hasOutPutFunctionV = hasOutPutFunction<T>::value;

/// Output function for std::pair
template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& ptr);

template <typename T, typename Cont>
auto outputElement(std::ostream& os, const T& element, const Cont& cont, ...) -> decltype(os);

template <typename T, typename = std::enable_if<!hasOutPutFunctionV<T>>>
auto operator<<(std::ostream& os, const T& container) -> decltype(container.begin(), container.end(), os)
{
    using std::decay_t;
    using std::is_same_v;
    using elementType = decay_t<decltype(*container.begin())>;
    constexpr bool isCharV = is_same_v<elementType, char>;
    if constexpr (!isCharV)
    {
        os << "{ ";
    }
    if (!container.empty())
    {
        auto end = container.end();
        bool onFirstElement = true;
        for (auto it = container.begin(); it != end; ++it)
        {
            if constexpr (isCharV)
            {
                if (*it == '\0')
                {
                    break;
                }
            }
            if constexpr (!isCharV)
            {
                if (!onFirstElement)
                {
                    os << ", ";
                }
                else
                {
                    onFirstElement = false;
                }
            }
            outputElement(os, *it, container, isPair<elementType>());
        }
    }
    if constexpr (!isCharV)
    {
        os << " }";
    }
    return os;
}

template <typename T, typename Cont>
auto outputElement(std::ostream& os, const T& element, const Cont&, std::true_type)
    -> decltype(std::declval<typename Cont::key_type>(), os)
{
    os << element.first << " ==> " << element.second;
}

template <typename T, typename Cont>
auto outputElement(std::ostream& os, const T& element, const Cont&, ...)
    -> decltype(os)
{
    os << element;
    return os;
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& pr)
{
    os << '(' << pr.first << ", " << pr.second << ')';
    return os;
}

#endif //CPP_DEMO_OUTPUTCONTAINER_H
