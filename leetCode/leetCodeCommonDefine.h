//
// Created by william on 2022/6/9.
//

#ifndef CPP_DEMO_LEETCODECOMMONDEFINE_H
#define CPP_DEMO_LEETCODECOMMONDEFINE_H
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

// 链表数据结构
struct ListNode
{
    int val;
    ListNode* next;
    ListNode() :
        val(0), next(nullptr) {}
    explicit ListNode(int x) :
        val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) :
        val(x), next(next) {}
};

static void trimLeftTrailingSpaces(std::string& input)
{
    input.erase(input.begin(), find_if(input.begin(), input.end(), [](int ch) {
                    return !isspace(ch);
                }));
}

static void trimRightTrailingSpaces(std::string& input)
{
    input.erase(find_if(input.rbegin(), input.rend(), [](int ch) {
                    return !isspace(ch);
                }).base(),
                input.end());
}

static std::vector<int> stringToIntegerVector(std::string input)
{
    std::vector<int> output;
    trimLeftTrailingSpaces(input);
    trimRightTrailingSpaces(input);
    input = input.substr(1, input.length() - 2);
    std::stringstream ss;
    ss.str(input);
    std::string item;
    char delim = ',';
    while (getline(ss, item, delim))
    {
        output.push_back(stoi(item));
    }
    return output;
}

static int stringToInteger(const std::string& input)
{
    return stoi(input);
}

static std::string integerVectorToString(std::vector<int> list, int length = -1)
{
    if (length == -1)
    {
        length = list.size();
    }

    if (length == 0)
    {
        return "[]";
    }

    std::string result;
    for (int index = 0; index < length; index++)
    {
        int number = list[index];
        result += std::to_string(number) + ", ";
    }
    return "[" + result.substr(0, result.length() - 2) + "]";
}

static ListNode* stringToListNode(const std::string& input)
{
    // Generate list from the input
    std::vector<int> list = stringToIntegerVector(input);
    // Now convert that list into linked list
    auto* dummyRoot = new ListNode(0);
    auto* ptr = dummyRoot;
    for (int item : list)
    {
        ptr->next = new ListNode(item);
        ptr = ptr->next;
    }
    ptr = dummyRoot->next;
    delete dummyRoot;
    return ptr;
}

static std::string listNodeToString(ListNode* node)
{
    if (node == nullptr)
    {
        return "[]";
    }

    std::string result;
    while (node)
    {
        result += std::to_string(node->val) + ", ";
        node = node->next;
    }
    return "[" + result.substr(0, result.length() - 2) + "]";
}

#endif //CPP_DEMO_LEETCODECOMMONDEFINE_H
