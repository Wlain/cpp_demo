//
// Created by william on 2022/6/9.
//

#include "leetCodeCommonDefine.h"

/*
 * difficulty：medium
 * 解法一：迭代法
 */

//[2,4,3, 1]
//[5,6,4]

class Solution
{
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2)
    {
        auto* temp = new ListNode(0);
        auto* result = temp;
        int sum{ 0 };
        int num{ 0 };
        while (l1 != nullptr && l2 != nullptr)
        {
            sum = l1->val + l2->val + num;
            num = sum / 10;
            result->next = new ListNode(sum % 10);
            result = result->next;
            l1 = l1->next;
            l2 = l2->next;
            if (num > 0 && !l1 && !l2)
            {
                result->next = new ListNode(num);
                result = result->next;
            }
        }
        while (l1 != nullptr)
        {
            sum = l1->val + num;
            num = sum / 10;
            result->next = new ListNode(sum % 10);
            result = result->next;
            l1 = l1->next;
            if (num > 0 && !l1)
            {
                result->next = new ListNode(num);
                result = result->next;
            }
        }
        while (l2 != nullptr)
        {
            sum = l2->val + num;
            num = sum / 10;
            result->next = new ListNode(sum % 10);
            result = result->next;
            l2 = l2->next;
            if (num > 0 && !l2)
            {
                result->next = new ListNode(num);
                result = result->next;
            }
        }
        result = temp->next;
        delete temp;
        return result;
    }
};

void addTwoNumbersTest()
{
    std::string line;
    while (getline(std::cin, line))
    {
        ListNode* l1 = stringToListNode(line);
        getline(std::cin, line);
        ListNode* l2 = stringToListNode(line);
        ListNode* ret = Solution().addTwoNumbers(l1, l2);
        std::string out = listNodeToString(ret);
        std::cout << out << std::endl;
    }
}