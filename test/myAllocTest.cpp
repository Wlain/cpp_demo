//
// Created by william on 2022/5/16.
//

#include <new>
#include <stdlib.h>
#include <iostream>
#include <vector>

/// 可以作为面试题

template <typename T>
struct MyAlloc
{
    using value_type = T;
    MyAlloc() {}

    template <typename U>
    MyAlloc(const MyAlloc<U>&) {}

    bool operator==(const MyAlloc&) const { return true; }
    bool operator!=(const MyAlloc&) const { return false; }

    T* allocate(const size_t n) const
    {
        if (n == 0)
        {
            return nullptr;
        }

        if(n > static_cast<size_t>(-1) / sizeof(T))
        {
            throw std::bad_array_new_length();
        }
        void* const pv = malloc(n * sizeof(T));
        if(!pv)
        {
            throw std::bad_alloc();
        }
        return static_cast<T*>(pv);
    }

    void deallocate(T* const p, size_t) const
    {
        free(p);
    }
};

using MyIntVector = std::vector<int, MyAlloc<int>>;

void myAllocTest()
{
    MyIntVector foov = { 1701, 1764, 1664 };
    for (auto a : foov)
    {
        std::cout << a << " ";
    }
    std::cout <<"\n";
}