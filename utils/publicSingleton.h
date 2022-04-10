//
// Created by william on 2022/4/11.
//

#ifndef CPP_DEMO_PUBLICSINGLETON_H
#define CPP_DEMO_PUBLICSINGLETON_H
#include <type_traits>

template <typename T>
class PublicSingleton
{
protected:
    PublicSingleton() = default;

public:
    static T& getInstance() noexcept(std::is_nothrow_constructible<T>::value)
    {
        static T instance;
        return instance;
    }
    virtual ~PublicSingleton() noexcept = default;
    PublicSingleton(const PublicSingleton&) = delete;
    PublicSingleton& operator=(const PublicSingleton&) = delete;
};

#endif //CPP_DEMO_PUBLICSINGLETON_H
