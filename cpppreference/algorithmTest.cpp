//
// Created by william on 2022/5/17.
//
#include "outputContainer.h"

#include <algorithm>
#include <functional> // greater<int>( )
#include <iostream>
#include <list>
#include <vector>

bool twice(int elem1, int elem2)
{
    return elem1 * 2 == elem2;
}

void printLine()
{
    std::cout << "===============================" << std::endl;
}

bool modLesser(int elem1, int elem2)
{
    if (elem1 < 0)
        elem1 = -elem1;
    if (elem2 < 0)
        elem2 = -elem2;
    return elem1 < elem2;
}

template <typename Type>
class MultValue
{
public:
    MultValue(const Type& value) :
        m_factor(value) {}
    // The function call for the element to be multiplied
    Type operator()(Type& elem) const
    {
        return elem * m_factor;
    }

private:
    Type m_factor;
};

void algorithmTest()
{
    using std::cout;
    using std::endl;
    /// adjacent_find:搜索相等或满足指定条件的两个相邻元素。是一种非变异序列算法
    std::list<int> l;
    std::list<int>::iterator result1, result2;
    l.push_back(50);
    l.push_back(40);
    l.push_back(10);
    l.push_back(20);
    l.push_back(20);
    std::cout << l << std::endl;
    result1 = std::adjacent_find(l.begin(), l.end());
    if (result1 == l.end())
        std::cout << "There are not two adjacent elements that are equal." << std::endl;
    else
        std::cout << "There are two adjacent elements that are equal.\n"
                  << "They have a value of "
                  << *(result1) << "." << std::endl;
    result2 = std::adjacent_find(l.begin(), l.end(), twice);
    if (result2 == l.end())
        std::cout << "There are not two adjacent elements where the "
                  << "second is twice the first." << std::endl;
    else
        std::cout << "There are two adjacent elements where "
                  << "the second is twice the first.\n"
                  << "They have values of " << *(result2++)
                  << " & " << *result2 << "." << std::endl;
    printLine();
    /// all_of 当给定范围内的每个元素都存在条件时返回true
    auto isEven = [](int elem) { return !(elem % 2); };
    if (std::all_of(l.begin(), l.end(), isEven))
        cout << "All the elements are even numbers.\n";
    else
        cout << "Not all the elements are even numbers.\n";
    printLine();
    /// any_of:当条件在指定的元素范围内至少出现一次时返回true
    auto hasValue = [](int const elem) { return elem == 20; };
    if (std::any_of(l.begin(), l.end(), hasValue))
        cout << "There's an element 20 in l.\n";
    else
        cout << "There are no element 20 in l.\n";
    printLine();
    /// binary_search:测试排序范围内是否存在等于指定值
    std::list<int> list1;
    list1.push_back(50);
    list1.push_back(10);
    list1.push_back(30);
    list1.push_back(20);
    list1.push_back(25);
    list1.push_back(5);
    list1.sort();
    cout << list1 << std::endl;
    // default binary search for 10
    if (std::binary_search(list1.begin(), list1.end(), 10))
        cout << "There is an element in list List1 with a value equal to 10."
             << endl;
    else
        cout << "There is no element in list List1 with a value equal to 10."
             << endl;
    // a binary_search under the binary predicate greater
    list1.sort(std::greater<>());
    if (std::binary_search(list1.begin(), list1.end(), 10, std::greater<int>()))
        cout << "There is an element in list List1 with a value greater than 10 "
             << "under greater than." << endl;
    else
        cout << "No element in list List1 with a value greater than 10 "
             << "under greater than." << endl;
    // a binary_search under the user-defined binary predicate mod_lesser
    std::vector<int> v1;
    for (auto i = -2; i <= 4; ++i)
    {
        v1.push_back(i);
    }
    sort(v1.begin(), v1.end(), modLesser);
    cout << v1 << endl;
    if (std::binary_search(v1.begin(), v1.end(), -3, modLesser))
        cout << "There is an element with a value equivalent to -3 "
             << "under mod_lesser." << endl;
    else
        cout << "There is not an element with a value equivalent to -3 "
             << "under mod_lesser." << endl;
    /// clamp:std::clamp(x, min, max)
    printLine();
    /// copy:
    // first:一个输入迭代器，用于处理源范围中第一个元素的位置。
    // last: 一个输入迭代器，用于处理源范围中最后一个元素之后的位置。
    // result:一个输出迭代器，用于处理目标范围中第一个元素的位置,迭代器寻址result+ ( last- first)。
    std::vector<int> v2, v3;
    for (int i = 0; i <= 5; i++)
        v2.push_back(10 * i);
    cout << "v2:" << v2 << endl;
    for (int i = 0; i <= 10; i++)
        v3.push_back(3 * i);
    cout << "v3:" << v3 << endl;
    /// To copy the first 3 elements of v1 into the middle of v2
    std::copy(v2.begin(), v2.begin() + 3, v3.begin() + 4);
    cout << "v3 with v2 insert = " << v3 << std::endl;
    // To shift the elements inserted into v2 two positions
    // to the left
    std::copy(v3.begin() + 4, v3.begin() + 7, v3.begin() + 2);
    cout << "v3 with shifted insert = " << v3 << endl;
    printLine();
    /// transform:
    /// 将指定的函数对象应用于源范围中的每个元素或两个源范围中的一对元素，并将函数对象的返回值复制到目标范围中
    v1.clear();
    v2.clear();
    v3.clear();
    for (int i = -4; i <= 2; i++)
    {
        v1.push_back(i);
    }
    cout << "Original vector v1 =" << v1 << endl;
    // Modifying the vector v1 in place
    std::transform(v1.begin(), v1.end(), v1.begin(), MultValue<int>(2));
    cout << "The elements of the vector v1 multiplied by 2 in place gives:"
         << "\n v1mod = ( " << v1 << "\n";
    v2.resize(7);
    // Using transform to multiply each element by a factor of 5
    std::transform(v1.begin(), v1.end(), v2.begin(), MultValue<int>(5));
    cout << "Multiplying the elements of the vector v1mod\n "
         << "by the factor 5 & copying to v2 gives:\n v2 = " << v2 << std::endl;
    // The second version of transform used to multiply the
    // elements of the vectors v1mod & v2 pairwise
    v3.resize(7);
    std::transform(v1.begin(), v1.end(), v2.begin(), v3.begin(), std::multiplies<>());
    cout << "Multiplying elements of the vectors v1mod and v2 pairwise "
         << "gives:\n v3 = " << v3 << std::endl;
}