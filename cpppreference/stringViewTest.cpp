//
// Created by william on 2021/4/16.
//
/// 默认构造函数： data() 等于 nullptr ，而 size() 等于 0
/// 复制构造函数:  data() 等于 other.data() ，而 size() 等于 other.size() 。

#include "base.h"

void stringViewTest()
{
    std::wstring_view wcstr_v = L"xyzzy";
    char array[3] = {'B', 'a', 'r'};
    std::string_view array_v(array, std::size(array));
    std::string cppstr = "Foo";
    std::string_view cppstr_v(&cppstr[0], cppstr.size());
    std::cout << cppstr_v << '\n'
              << array_v << '\n'
              << wcstr_v.size() << '\n';
    std::cout << wcstr_v.data() << std::endl;
    std::string_view v = "Hello, world";
    v = v.substr(7);
    std::cout << v << '\n';
    std::string_view str_view("abcd");
    auto begin = str_view.begin();
    auto cbegin = str_view.cbegin();
    std::cout << *begin << '\n';
    std::cout << *cbegin << '\n';
    std::cout << std::boolalpha << (begin == cbegin) << '\n';
    std::cout << std::boolalpha << (*begin == *cbegin) << '\n';
    std::string str = "   trim me";
    std::string_view v1 = str;
    v1.remove_prefix(std::min(v.find_first_not_of(" "), v.size()));
    std::cout << "String: '" << str << "'\n"
              << "View  : '" << v1 << "'\n";
    constexpr std::basic_string_view<char> source{"ABCDEF"};
    std::array<char, 8> dest;
    std::size_t count{}, pos{};
    dest.fill('\0');
    source.copy(dest.data(), count = 4); // pos = 0
    std::cout << dest.data() << '\n'; // ABCD
    dest.fill('\0');
    source.copy(dest.data(), count = 4, pos = 1);
    std::cout << dest.data() << '\n'; // BCDE
    dest.fill('\0');
    source.copy(dest.data(), count = 42, pos = 2); // ok, count -> 4
    std::cout << dest.data() << '\n'; // CDEF
    try
    {
        source.copy(dest.data(), count = 1, pos = 666); // throws: pos > size()
    }
    catch(std::out_of_range const& ex)
    {
        std::cout << ex.what() << '\n';
    }
    using namespace std::literals;
    constexpr auto data{"ABCDEF"sv};
    std::cout << data.substr() << '\n'; // ABCDEF
    std::cout << data.substr(pos = 1) << '\n'; // BCDEF
    std::cout << data.substr(pos = 2, count = 3) << '\n'; // CDE
    std::cout << data.substr(pos = 4, count = 42) << '\n'; // EF
    // count -> 2 == size() - pos == 6 - 4

}