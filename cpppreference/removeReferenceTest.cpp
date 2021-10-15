//
// Created by william on 2021/7/17.
//

#include "base.h"

// If the type T is a reference type, provides the member typedef type which is the type referred to by T. Otherwise type is T.

template<class T1, class T2>
void printTheSame()
{
    std::cout << std::is_same<T1, T2>() << std::endl;
}

void removeReferenceTest()
{
    std::cout << std::boolalpha;
    printTheSame<int, int>();
    printTheSame<int, int&>();
    printTheSame<int, int&&>();

    printTheSame<int, std::remove_reference<int>::type>();
    printTheSame<int, std::remove_reference<int&>::type>();
    printTheSame<int, std::remove_reference<int&&>::type>();


}