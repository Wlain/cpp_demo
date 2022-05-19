//
// Created by william on 2022/5/17.
//
#include "outputContainer.h"

#include <algorithm>
#include <functional> // greater<int>( )
#include <iostream>
#include <list>
#include <queue>
#include <random>
#include <string>
#include <vector>

bool twice(int elem1, int elem2)
{
    return elem1 * 2 == elem2;
}

void printLine(std::string_view type)
{
    std::cout << "===============" << type.data() << "===============" << std::endl;
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

template <typename T>
void dumpVector(const std::vector<T>& v, std::pair<typename std::vector<T>::iterator, typename std::vector<T>::iterator> range)
{
    // prints vector v with range delimited by [ and ]
    for (auto i = v.begin(); i != v.end(); ++i)
    {
        if (i == range.first)
        {
            std::cout << "[ ";
        }
        if (i == range.second)
        {
            std::cout << "] ";
        }

        std::cout << *i << " ";
    }
    std::cout << std::endl;
}

template <typename T>
void equalRangeDemo(const std::vector<T>& originalVector, T value)
{
    std::vector<T> v(originalVector);
    sort(v.begin(), v.end());
    std::cout << "Vector sorted by the default binary predicate <:" << std::endl
              << '\t';
    for (auto i = v.begin(); i != v.end(); ++i)
    {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
    std::pair<typename std::vector<T>::iterator, typename std::vector<T>::iterator> result = std::equal_range(v.begin(), v.end(), value);
    std::cout << "Result of equal_range with value = " << value << ":" << std::endl
              << '\t';
    dumpVector(v, result);
    std::cout << std::endl;
}

template <typename T, typename F>
void equalRangeDemo(const std::vector<T>& originalVector, T value, F pred, std::string predName)
{
    using std::cout;
    using std::endl;
    std::vector<T> v(originalVector);
    sort(v.begin(), v.end(), pred);
    cout << "Vector sorted by the binary predicate " << predName.c_str() << ":" << endl
         << '\t';
    for (auto i = v.begin(); i != v.end(); ++i)
    {
        cout << *i << " ";
    }
    cout << endl;
    auto result = std::equal_range(v.begin(), v.end(), value, pred);
    cout << "Result of equal_range with value = " << value << ":" << endl
         << '\t';
    dumpVector(v, result);
    cout << endl;
}

// Return whether absolute value of elem1 is less than absolute value of elem2
bool absLesser(int elem1, int elem2)
{
    return abs(elem1) < abs(elem2);
}

// Return whether string l is shorter than string r
bool shorterThan(const std::string& l, const std::string& r)
{
    return l.size() < r.size();
}

void algorithmTest()
{
    using std::cout;
    using std::endl;
    /// adjacent_find:搜索相等或满足指定条件的两个相邻元素。是一种非变异序列算法
    printLine("adjacent_find");
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
    /// all_of 当给定范围内的每个元素都存在条件时返回true
    printLine("all_of");
    auto isEven = [](int elem) { return !(elem % 2); };
    if (std::all_of(l.begin(), l.end(), isEven))
        cout << "All the elements are even numbers.\n";
    else
        cout << "Not all the elements are even numbers.\n";
    /// any_of:当条件在指定的元素范围内至少出现一次时返回true
    printLine("any_of");
    auto hasValue = [](int const elem) { return elem == 20; };
    if (std::any_of(l.begin(), l.end(), hasValue))
        cout << "There's an element 20 in l.\n";
    else
        cout << "There are no element 20 in l.\n";
    /// binary_search:测试排序范围内是否存在等于指定值
    printLine("binary_search");
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
    printLine("clamp");
    int x = 2;
    std::cout << "int x = 2, std::clamp(x, 5, 10) is:" << std::clamp(x, 5, 10) << endl;
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
    /// copy_backward:将元素的值从源范围分配到目标范围，遍历元素的源序列并向后分配它们的新位置
    printLine("copy_backward");
    v2.clear();
    v3.clear();
    for (int i = 0; i <= 5; i++)
        v2.push_back(10 * i);
    cout << "v2:" << v2 << endl;
    for (int i = 0; i <= 10; i++)
        v3.push_back(3 * i);
    cout << "v2:" << v2 << endl;
    cout << "v3:" << v3 << endl;
    // To copy_backward the first 3 elements of v1 into the middle of v2
    std::copy_backward(v2.begin(), v2.begin() + 3, v3.begin() + 7);
    cout << "v3 with v2 insert = " << v3 << std::endl;
    // To shift the elements inserted into v2 two positions
    // to the right
    std::copy_backward(v3.begin() + 4, v3.begin() + 7, v3.begin() + 9);
    cout << "v2 with shifted insert = " << v3 << std::endl;
    /// transform:这个方法虽然很好，但是效率不是很高
    /// 将指定的函数对象应用于源范围中的每个元素或两个源范围中的一对元素，并将函数对象的返回值复制到目标范围中
    printLine("transform");
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
    /// copy_if:在一系列元素中，复制true用于指定条件的元素,返回复制元素的数量
    printLine("copy_if");
    std::list<int> li{ 46, 59, 88, 72, 79, 71, 60, 5, 40, 84 };
    std::list<int> le(li.size()); // le = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    std::list<int> lo(li.size()); // lo = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    cout << "li = " << li << endl;
    auto ec = std::copy_if(li.begin(), li.end(), le.begin(), isEven);
    le.resize(std::distance(le.begin(), ec)); // shrink le to new size
    cout << "Even numbers are le = " << le << endl;
    // is_odd checks if the element is odd.
    auto isOdd = [](int const elem) { return (elem % 2); };
    auto oc = std::copy_if(li.begin(), li.end(), lo.begin(), isOdd);
    lo.resize(std::distance(lo.begin(), oc)); // shrink le to new size
    cout << "Odd numbers are lo = " << lo << endl;
    /// copy_n:复制指定数量的元素
    printLine("copy_n");
    std::string s1{ "dandelion" };
    std::string s2{ "badger" };
    cout << s1.c_str() << " + " << s2.c_str() << " = ";
    std::copy_n(s1.begin(), 3, s2.begin());
    cout << s2.c_str() << endl;
    /// count:返回范围内其值与指定值匹配的元素数
    printLine("count");
    v1.clear();
    v1.push_back(10);
    v1.push_back(20);
    v1.push_back(10);
    v1.push_back(40);
    v1.push_back(10);
    cout << "v1 = " << v1 << endl;
    std::vector<int>::iterator::difference_type result = std::count(v1.begin(), v1.end(), 10);
    cout << "The number of 10s in v2 is: " << result << "." << endl;
    /// count_if:返回范围内其值满足指定条件的元素的数量
    printLine("count_if");
    auto count = count_if(v1.begin(), v1.end(), [](int elem) { return 10; });
    cout << "The number of elements in v1 greater than 10 is: " << count << "." << endl;
    /// equal:在二元谓词指定的意义上，逐个元素比较两个范围的相等性或等价性
    printLine("equal");
    v1 = { 0, 5, 10, 15, 20, 25 };
    v2 = { 0, 5, 10, 15, 20, 25 };
    v3 = { 0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50 };
    // Using range-and-a-half equal:
    bool b = equal(v1.begin(), v1.end(), v2.begin());
    cout << "v1 and v2 are equal: " << std::boolalpha
         << b << endl; // true, as expected
    b = equal(v1.begin(), v1.end(), v3.begin());
    cout << "v1 and v3 are equal: " << std::boolalpha
         << b << endl; // true, surprisingly
    // Using dual-range equal:
    b = equal(v1.begin(), v1.end(), v3.begin(), v3.end());
    cout << "v1 and v3 are equal with dual-range overload: " << std::boolalpha
         << b << endl; // false
    /// equal_range:给定一个有序范围，找到其中所有元素都等于给定值的子范围//TODO
    printLine("equal_range");
    v1.clear();
    // Constructing vector v1 with default less than ordering
    for (int i = -1; i <= 4; ++i)
    {
        v1.push_back(i);
    }
    for (int i = -3; i <= 0; ++i)
    {
        v1.push_back(i);
    }
    equalRangeDemo(v1, 3);
    equalRangeDemo(v1, 3, std::greater<>(), "greater");
    equalRangeDemo(v1, 3, absLesser, "absLesser");
    /// fill:为指定范围内的每个元素分配相同的新值。
    printLine("fill");
    v1.clear();
    for (int i = 0; i <= 9; i++)
    {
        v1.push_back(5 * i);
    }
    cout << "Vector v1 =" << v1 << endl;
    // Fill the last 5 positions with a value of 2
    std::fill(v1.begin() + 5, v1.end(), 2);
    cout << "Modified v1 =" << v1 << endl;
    /// generate:将函数对象生成的值分配给范围内的每个元素.
    v1.resize(5);
    std::deque<int> deq1(5);
    std::generate(v1.begin(), v1.end(), std::rand);
    cout << "Vector v1 =" << v1 << endl;
    std::generate(deq1.begin(), deq1.end(), std::rand);
    cout << "Deque deq1 is " << deq1 << endl;
    /// generate_n:将函数对象生成的值分配给范围内指定数量的元素，并返回到最后一个分配值之后的位置
    printLine("generate_n");
    constexpr const int elemcount = 5;
    std::vector<int> v(elemcount);
    std::deque<int> dq(elemcount);
    // Set up random number distribution
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<int> dist(-9, 9);
    // Call generate_n, using a lambda for the third parameter
    std::generate_n(v.begin(), elemcount, [&]() { return dist(engine); });
    cout << "Vector v1 =" << v << endl;
    generate_n(dq.begin(), elemcount, [&]() { return dist(engine); });
    cout << "deque dq =" << v << endl;
    /// remove:从给定范围中消除指定值，而不会干扰其余元素的顺序并返回没有指定值的新范围的结尾
    printLine("remove");
    v1.clear();
    for (int i = 0; i <= 9; i++)
        v1.push_back(i);
    for (int i = 0; i <= 9; i++)
        v1.push_back(7);
    /// std::random_shuffle was deprecated in C++14 and removed in C++17
    //    std::random_shuffle(v1.begin(), v1.end());
    auto newEnd = remove(v1.begin(), v1.end(), 7);
    cout << "Vector v1 with value 7 removed is " << v1 << endl;
    // To change the sequence size, use erase
    v1.erase(newEnd, v1.end());
    cout << "Vector v1 resized with value 7 removed is" << v1 << endl;
}
