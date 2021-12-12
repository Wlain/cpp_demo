//
// Created by william on 2021/12/12.
//
#include "geekTimeCpp/common/outputContainer.h"

#include <iostream>
#include <map>
#include <vector>

using namespace std;

namespace geekTimeTest
{
void outputContainerTest()
{
    map<int, int> mp{{1, 1}, {2, 4}, {3, 9}};
    cout << mp << endl;
    vector<vector<int>> vv{{1, 1}, {2, 4}, {3, 9}};
    cout << vv << endl;
}
} // namespace geekTimeTest