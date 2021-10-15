//
// Created by william on 2021/4/1.
//

#include "base.h"

using namespace std;

void init03()
{
    int x3 = { 3 };
    double x4 = { 3.0 };

    struct point
    {
        int x, y;
    };

    point p1 = { 1, 2 };
    (void)p1.x;
    (void)p1.y;

    // Mutable
    struct structWithMutable
    {
        structWithMutable(){};
        mutable int m;
    } const constStructWithMutable;
    constStructWithMutable.m = 2;
    int y = 0;
    int& ref = y;
}

template <class C>
void testGenerateContainer(C& c)
{
    assert(c.empty());
    assert(c.max_size() > 1000);
    c.push_back(0);
    assert(c.front() == 0);
    assert(c.back() == 0);
    assert(c.size() == 1);
    c.erase(c.begin());
    c.push_back(1);
    c.erase(c.begin());
    c.assign(4, 1);
    assert(c.size() == 4);
    c.clear();
}

template <class V>
void testVectorContainer(V& v)
{
    string err;
    try
    {
        v.at(666) = 0;
    }
    catch (std::out_of_range const& exc)
    {
        err = exc.what();
    }
    assert(!err.empty());
    assert(v[0] == 1);
    v.resize(4);
    int arr[] = { 1, 2, 3 };
    v.insert(v.begin(), arr, arr + 3);
    assert(v[1] == 2);
}

void container03()
{
    std::list<int> l;
    testGenerateContainer(l);

    std::vector<int> v;
    testGenerateContainer(v);
    testVectorContainer(v);

    v.reserve(10);
    assert(v.capacity() == 10);

    std::deque<int> d;
    testGenerateContainer(d);
    testVectorContainer(d);

    stack<int> s;
    queue<int> q;
}

bool func(int i, int j)
{
    return i < j;
}

struct _
{
    bool operator()(int i, int j) { return i < j; }
} functor;

void sort03()
{
    int a[] = { 5, 7, 4, 2, 8, 6, 1, 9, 0, 3 };
    std::vector<int> v(a, a + sizeof a / sizeof a[0]);

    std::sort(v.begin(), v.end());
    std::sort(v.begin(), v.end(), std::greater<int>());
    std::sort(v.begin() + 4, v.end(), func);
    std::sort(v.begin(), v.end(), std::greater<int>());
    std::sort(v.begin(), v.end(), functor);

    int prev = -1;
    for (vector<int>::iterator i = v.begin(); i != v.end(); ++i)
    {
        assert(*i > prev);
        prev = *i;
    }
}

int& aRef(int& a)
{
    return a;
}

struct Common
{
    int n;
    Common(int x) :
        n(x)
    {}
};

struct VirtualA : virtual Common
{
    VirtualA() :
        Common(1) {}
};

struct VirtualB : virtual Common
{
    VirtualB() :
        Common(2) {}
};

struct Diamond : VirtualA, VirtualB
{
    Diamond(): Common(3), VirtualA(), VirtualB(){}
};

void cpp03Test()
{
    int a = 0;
    assert(typeid(int) == typeid(a));
    assert(typeid(int).name() == std::string("i"));
    aRef(a) = 2;
    assert(a == 2);

    Diamond d;
    assert(d.VirtualA::n == 3);
    assert(d.VirtualB::n == 3);
}