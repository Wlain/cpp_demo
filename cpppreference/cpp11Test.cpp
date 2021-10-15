//
// Created by william on 2021/4/1.
//

#include "base.h"

using namespace std;

void types11()
{
    static_assert(__cpp_decltype, "");
    int a;
    decltype(a) b;

    assert((is_same<decltype(a), decltype(b)>::value));
    assert((!is_same<decltype(a), unsigned>::value));
    assert((is_same<int, int32_t>::value));
    assert((is_same<signed, int32_t>::value));
    assert(is_integral<int>::value);
    assert(is_integral<bool>::value);
    assert(!is_integral<float>::value);
    assert(is_pointer<int*>::value);
    assert(sizeof(long long) >= 8);
}

void dynamicMemory11()
{
    int d = 0;
    std::unique_ptr<int> u1;
    assert(!u1);
    u1.reset(&d);
    assert(u1);
    *u1 = 1;
    assert(d == 1);

    std::unique_ptr<int> u2;

    int* p1 = u1.get();
    u2 = move(u1);
    assert(u2.get() == p1);
    assert(u2);
    assert(!u1);
    assert(u2.get() == &d);
    u2.release();
    u2.reset(new int(10));
    assert(*u2 == 10);

    u2.reset(); // deletes int(10)
    assert(u2 == nullptr);
    assert(!u2);

    std::shared_ptr<int> s1;
    assert(!s1);
    assert(!s1.use_count());
    auto s2 = make_shared<int>(1);
    assert(s2.use_count() == 1);

    s1 = s2;
    assert(s1.use_count() == 2);
    *s1 = 2;
    assert(*s1 == *s1.get());
    assert(*s2 == 2);

    s2.reset();
    assert(s1.use_count() == 1);
    assert(!s2.use_count());
}

char funcType(const int& x)
{
    assert(is_const<typename remove_reference<decltype(x)>::type>::value);
    assert(is_lvalue_reference<decltype(x)>::value);
    return 'C';
}

char funcType(int& x)
{
    assert(is_lvalue_reference<decltype(x)>::value);
    return 'L';
}

char funcType(int&& x)
{
    assert(is_rvalue_reference<decltype(x)>::value);
    return 'R';
}

template <class T>
char funcTypeTemplate(T&& x) // x is a forwarding reference
{
    // x is not R-value here
    assert(funcType(x) != 'R');
    // x can be forwarded as R or L value
    // https://en.cppreference.com/w/cpp/utility/forward
    return funcType(forward<T>(x)); // like func_type((T)(x));
}

void references11()
{
    // https://en.cppreference.com/w/cpp/language/reference
    assert(is_reference<int&>::value);

    // L-value:
    assert(is_lvalue_reference<int&>::value);

    // R-value
    assert(is_rvalue_reference<int&&>::value);

    const int c = 1;
    int i;
    assert(funcType(c) == 'C');
    assert(funcType(i) == 'L');
    assert(funcType(1) == 'R');
    assert(funcTypeTemplate(c) == 'C');
    assert(funcTypeTemplate(i) == 'L');
    assert(funcTypeTemplate(1) == 'R');
}

void init11()
{
    class C
    {
    public:
        int a, b, c;
    };

    auto o2 = C{ 1, 2, 3 };
    C o3{ 1, 2, 3 };
    (void)o3;

    auto z1 = C();
    C z2 = {};
    auto z3 = C{};

    assert(!z1.a);
    assert(!z2.a);
    assert(!z3.a);
}

auto autoInt = 1;

// int before(int a) { return a; }
auto trailingReturnType(int a) -> int
{
    return a;
}

void func11()
{
    class functor
    {
        int y = 1;

    public:
        int operator()(int a) const
        {
            return a + y;
        }
    };
    functor ft;
    assert(ft(1) == 2);

    function<int(int)> ft2 = ft;
    assert(ft(2) == 3);
    return;
    //    auto binded = std::bind(ft2, 3);
    auto binded = [ft2] { return ft2(3); };
    assert(binded() == 5);
}

static_assert(__cpp_constexpr, "");

constexpr int constexprFactorial(int n)
{
    return n < 1 ? 1 : (n * constexprFactorial(n - 1));
}

template <typename T>
T constexpr adder(T v)
{
    return v;
}

template <typename T, typename... Args>
T constexpr adder(T first, Args... args)
{
    return first + adder(args...);
}

static_assert(adder(1, 2, 3) == 6, "");

struct Base11
{
    virtual void method1();
    virtual void method2();
};

struct Derived11 : Base11
{
    void method1() override; ///< [ref](https://en.cppreference.com/w/cpp/language/override)
    void method2() final;    ///< [ref](https://en.cppreference.com/w/cpp/language/final)
};

static void lambdaBasics(void)
{
    auto annotatedNamedLambdaExpression = // optional name
        []                                // capture
        ()                                // optional list of arguments
    {};                                   // body
    auto closure = [] {};
    closure();
    // with arguments
    auto pass = [](int a) { return a; };
    assert(pass(5) == 5);
    // lambda captures external value
    int c = 1;
    auto getI = [=]() { return c; };
    assert(getI() == 1);
    // lambda captures external variable by reference
    // with omitted arguments and return type
    auto incGet = [&] { return ++c; };
    assert(incGet() == 2);
    assert(incGet() == 3);

    // annotated expanded empty inline lambda call:
    []          // capture
        ()      // optional list of arguments
        -> void // optional return value
    {}          // body
    ();         // call with arguments

    // annotated expanded sample inline lambda call:
    c =               // result
        [c]           // capture
        (int a)       // optional list of arguments
        -> int        // optional return value
    { return c + a; } // body
    (1);              // call with argument
    assert(c == 4);

    // inline lambda which is called in place
    // https://en.wikipedia.org/wiki/Anonymous_function
    // assert((1 + 1) == 2);
    assert([](int a) { return a + 1; }(1) == 2);
}

/// @cond
static int glob;
/// @endcond

static void lambdaCapture(void)
{
    // read only
    int i = 2;
    assert([=] { return i; }() == 2);

    // read and write access
    [&](int a) { i = a; }(3);
    assert(i == 3);
    // explicit r/o and r/w

    int j;
    [i, &j]() { j = i; }();
    assert(j == i);

    // r/o by default
    i++;
    [=, &j]() { j = i; }();
    assert(j == i);

    // r/w by default
    i++;
    [&, i]() { j = i; }();
    assert(j == i);
    // can access globals anyway
    auto incGlobal = []() { return ++glob; };
    assert(incGlobal() == 1);
    assert(incGlobal() == 2);
}

void container11()
{
    // [list_initialization](https://en.cppreference.com/w/cpp/language/list_initialization)
    vector<int> v = { 1, 2, 3 };
    assert(v.data()[2] == 3);

    v.shrink_to_fit();
    v.emplace(v.cbegin(), 0);
    assert(v.front() == 0);
    v.emplace_back(4);
    assert(v.back() == 4);
    array<int, 1> a1, a2;
    swap(a1, a2);
    /// [forward_list](https://en.cppreference.com/w/cpp/container/forward_list)
    forward_list<int> fl;
}

void sort11()
{
    /// [array](https://en.cppreference.com/w/cpp/container/array)
    array<int, 10> s{ 5, 7, 4, 2, 8, 6, 1, 9, 0, 3 };
    sort(s.begin(), s.end(),
         // sort using a lambda expression
         [](int a, int b) { return a > b; });
}

// int use_lambda(int a; int (func*)(int))
static int useLambda(int a, function<int(int)> f)
{
    // lambda argument is like pointer to functions
    return f(a);
}

static function<int(int)> g_f;
static void setLambda(function<int(int)>&& f)
{
    g_f = std::move(f);
}

static int callLambda(int a)
{
    return g_f(a);
}

static void lambdaComplex(void)
{
    auto increment = [](int a) -> int { return a + 1; };
    assert(increment(5) == 6);
    // named lambda as argument
    assert(useLambda(2, increment) == 3);
    setLambda(increment);
    assert(callLambda(3) == 4);
    // inline lambda as argument
    assert(useLambda(1, [](int a) { return a + 1; }) == 2);
}

// just function with delay for demonstration
int lento(int a = 0)
{
    this_thread::yield();
    this_thread::sleep_for(chrono::milliseconds(20));
    return a+1;
}


void threads11()
{
    this_thread::yield();
    this_thread::get_id();
    this_thread::sleep_for(chrono::nanoseconds(1));
    promise<int> p;
    future<int> f = p.get_future();
    int v = 0;
    thread t([&p, &v]{
      lento();
      p.set_value(2);
      v = 3;
    });
    assert(v == 0);
    assert(f.get() == 2);
    lento();
    assert(v == 3);
    thread t2;
    t2.swap(t);
    assert(!t.joinable());
    assert(t2.joinable());
    t2.join();
    try {
        t2.join();
        t2.detach();
    } catch(const std::system_error& e) {
        assert(e.code().value() == 22);
    }
    assert(!t2.joinable());
    // detach demo
    {
        thread t3([&p, &v]{
          v = 4;
        });
        t3.detach();
    }
    lento();
    assert(v == 4);
}

void mutex11()
{
    int unguarded = 0, guarded = 0;
    mutex m;

    thread t1([&unguarded, &guarded, &m]{
      unguarded = lento(unguarded) + 1;
      lock_guard<mutex> guard(m);
      guarded = lento(guarded) + 1;
    });

    thread t2([&unguarded, &guarded, &m]{
      unguarded = lento(unguarded) + 1;

      lock_guard<mutex> guard(m);
      guarded = lento(guarded) + 1;
    });
    assert(unguarded == 0);
    assert(guarded == 0);
    t1.join();
    t2.join();
    assert(unguarded == 1);
    assert(guarded == 2);
}





void cpp11Test()
{
    init11();
    std::cout << "current cpp version is" << __cplusplus << std::endl;
    references11();
    init11();
    auto r = trailingReturnType(1);
    (void) r;
    lambdaBasics();
    lambdaCapture();
    lambdaComplex();
    func11();
    container11();
    sort11();
    dynamicMemory11();
    static_assert(constexprFactorial(4), "");

    types11();
    threads11();
    mutex11();
}
