//
// Created by william on 2021/12/13.
//
#include "geekTimeCpp/common/outputContainer.h"

#include <algorithm>  // std::sort
#include <functional> // std::less/greater/hash
#include <iostream>   // std::cout/endl
#include <string>     // std::string
#include <vector>     // std::vector

namespace geekTimeTest
{
void hashTest()
{
    using namespace std;
    vector<int> v{ 13, 6, 4, 11, 29 };
    cout << v << endl;
    sort(v.begin(), v.end());
    cout << v << endl;

    sort(v.begin(), v.end(), greater<int>());
    cout << v << endl;

    cout << hex;

    auto hp = hash<int*>();
    cout << "hash(nullptr) = " << hp(nullptr) << endl;
    cout << "hash(v.data)) = " << hp(v.data()) << endl;
    cout << "v.data()       = " << static_cast<void*>(v.data()) << endl;
    auto hs = hash<string>();
    cout << "hash(\"hello\")  = " << hs(string("hello")) << endl;
    cout << "hash(\"hellp\")  = " << hs(string("hellp")) << endl;
}
} // namespace geekTimeTest