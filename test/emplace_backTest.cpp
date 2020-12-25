//
// Created by william on 2020/12/24.
//

#include "base.h"
#include <vector>

// 参数value_type 必须满足可移动插入 (MoveInsertable) 和 可就位构造 (EmplaceConstructible) 的要求。

using namespace std;

struct President
{
    std::string name;
    std::string country;
    int year;

    President(std::string p_name, std::string p_country, int p_year) :
        name(std::move(p_name)), country(std::move(p_country)), year(p_year)
    {
        cout << "I am being constructed!" << endl;
    }

    President(President&& other)  noexcept :
        name(std::move(other.name)), country(std::move(other.country)), year(other.year)
    {
        cout << "I am being moved!" << endl;
    }
    President& operator=(const President& other) = default;
};

void emplace_backTest()
{
    std::vector<President> elections;
    cout << "emplace_back" << endl;
    elections.emplace_back("Nelson Mandela", "South Africa", 1994);
    std::vector<President> reElections;
    cout << "\npush_back:\n";
    reElections.push_back(std::move(President("Franklin Delano Roosevelt", "the USA", 1936)));
    cout << "\nContents\n";
    for (const auto& president : elections)
    {
        cout << president.name << "was elected president of "
        << president.country << "in " << president.year << ".\n";
    }
    for (const auto& president : reElections)
    {
        cout << president.name << "was re-elected president of "
             << president.country << "in " << president.year << ".\n";
    }

}
