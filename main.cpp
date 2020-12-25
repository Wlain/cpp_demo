//
// Created by william on 2020/7/1.
//
#include "include/base.h"
#include "include/basic_timer.h"
#include "vec2f.h"
#include "vector2.h"

#include <array>
#include <random>
#include <regex>

extern void testAuto();
extern void testDecltype();
extern void testConstantPointer();
extern void callbackTest();
extern void ifTest1();
extern void ifTest2();
extern void ifSwitch1();
extern void ifSwitch2();
extern void lambdaTest();
extern void testModern01();
extern void emplace_backTest();
extern void referenceTest();

int Vec2f::constructorCount = 0;
int Vec2f::destructorCount = 0;
// 必须在定义类的文件中对静态成员变量进行一次说明,或初始化。否则编译能通过，链接不能通过。
using namespace std;

class Base
{
public:
    Base()
    {
        std::cout << "Base call" << std::endl;
        temp = std::make_shared<float>();
    }

    Base(int a)
    {
        std::cout << "Base call : a" << std::endl;
    }

private:
    float a = 0;
    std::shared_ptr<float> temp;
    //    float b =0;
    //    float c =0;
};

class Derived : public Base
{
public:
    Derived() = default;
    Derived(int a){};
};

std::string getString()
{
    return std::string("123") + std::string("12223");
}

int main()
{
    //  testAuto();
    //  testDecltype();
    //  testConstantPointer();
    //    std::array<std::array<float, 4>, 9> colorValue;
    //    std::array<std::array<float, 2>, 2> color;
    //    float arr[2][2] = {1.0, 1.0, 1.0, 2.0};
    //    callbackTest();
    //        std::cout << c << std::endl;
    //    string pattern("[^c]ei");
    //    pattern = "[[:alpha:]]*" + pattern + "[[:alpha:]]*";
    //    regex r(pattern);
    //    smatch results;
    //    string test_str = "receipt freind theif receive";
    //    if (regex_search(test_str, results, r))
    //    {
    //        cout << results.str() << endl;
    //    }
    //    const std::string s = "@selcolor red(11, 0, 0, 0) green(1, 0, 0, 0)";
    //
    //    std::regex words_regex("(red|yellow|green|cyan|blue|magenta|white|gray|black)+([^rugcbmwgb]+)");
    //    auto words_begin =
    //        std::sregex_iterator(s.begin(), s.end(), words_regex);
    //    auto words_end = std::sregex_iterator();
    //
    //    std::cout << "Found "
    //              << std::distance(words_begin, words_end)
    //              << " words:\n";
    //
    //    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
    //        std::smatch match = *i;
    //        std::string match_str = match.str();
    //        std::cout << match_str << '\n';
    //    }

    //    vector<int> vec;
    //    for (int i = 0; i < 10; i++)
    //    {
    //        vec.push_back(i);
    //    }
    //    for (auto it = vec.begin(); it != vec.end();)
    //    {
    //        if (*it % 3 == 0)
    //            it = vec.erase(it); //删除元素，返回值指向已删除元素的下一个位置
    //        else
    //            ++it; //指向下一个位置
    //    }

    //    cout << Vec2f::getConstructorCount() << endl;
    //    std::random_device rd;
    //    std::default_random_engine gen = std::default_random_engine(rd());
    //    std::uniform_int_distribution<int> dis(1,10);
    //    srand((unsigned)time(nullptr));
    //    std::cout << "some random numbers between 1 and 10: ";
    //    cout << "------------(rand() % 800 / 1000.0f)-----------"
    //    for (int i=0; i<10; ++i)
    //        std::cout << (rand() % 800 / 1000.0f) << " ";
    //    std::cout << std::endl;
    //    以随机值播种，若可能
    //    for (int i = 0; i < 100; ++i)
    //    {
    //        cout << uniformDist(e1) << "\t";
    //    }
    Vector2 v1{ 1, 2 };
    Vector2 v2{ 3, 4 };
    const Vector2& v3 = v1 + v2;
    Vector2 rightDir = (v1 - v2);
    //    ifTest1();
    //    ifTest2();
    //    ifSwitch1();
    //    ifSwitch2();
    lambdaTest();
    // C++ 17 类型推导
    std::array arr{ 1, 2, 3, 4 }; // // -> std::array<int, 4>
    std::array m_attribs{ -1, -1 };
    std::array m_meshAttribs{ -1, -1 };

    std::reverse_copy(arr.begin(), arr.end(),
                      std::ostream_iterator<int>(std::cout, " "));

    std::cout << std::endl;
    std::vector vec{ 1, 2, 3 };
    std::reverse_copy(vec.begin(), vec.end(),
                      std::ostream_iterator<int>(std::cout, " "));
    std::cout << endl;
    testModern01();
    cout << endl;
    Base base;
    Derived derived;
    cout << sizeof(base) << endl;
    cout << sizeof(derived) << endl;
    emplace_backTest();
    const char* s = getString().c_str();
    cout << s << endl;
    referenceTest();
    return 0;
}
