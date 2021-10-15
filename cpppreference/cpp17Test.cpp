//
// Created by william on 2021/4/1.
//

#include "base.h"

#include <iostream>
#include <memory>

using namespace std;

void reference17()
{
    static_assert(is_reference_v<int&>);
    // L-value:
    static_assert(is_lvalue_reference_v<int&>);
    // R-value
    static_assert(is_rvalue_reference_v<int&&>);
}

#if __cpp_deduction_guides
/// @brief pair<int, double> p(1, 2.3);
constexpr pair deducted_pair(1, 2.3);
static_assert(deducted_pair.second == 2.3);

/// @brief auto t = make_tuple(4, 3, 2.5);
constexpr tuple deducted_tuple(4, 2, 2.5);
static_assert(get<2>(deducted_tuple) == 2.5);

/// @brief template with type float by default
template <typename T = float>
struct template_struct
{
    T val;
    template_struct() :
        val() {}
    template_struct(T val) :
        val(val) {}
};

/// @brief deducted \<int\>
template_struct template_arg_deduction{ 1 };

    #if __cpp_deduction_guides

/// @brief deducted \<float\>
template_struct template_default_arg_deduction;
    #endif

vector<int> int_vector = { 1, 2, 3, 4 };
/// @brief deduced deque\<int\>
deque deduction_guide1_queue(int_vector.begin(), int_vector.end());

/// @brief deduced deque\<vector\<int> :: iterator\>
deque deduction_guide2_queue{ int_vector.cbegin(), int_vector.cend() };

/// [deduction_guides](https://en.cppreference.com/w/cpp/container/array/deduction_guides)
array deduction_guide_array{ 1, 2, 3, 4 };

/// @brief deduced vector\<int\>
vector deduction_guide1_vector(int_vector.begin(), int_vector.end());
/// @brief deduced vector\<vector\<int\> :: iterator\>
vector deduction_guide2_vector{ int_vector.begin(), int_vector.end() };

void deduction_guides_17()
{
    assert(deduction_guide1_queue[0] == 1);
    assert(*deduction_guide2_queue[0] == 1);
    assert(deduction_guide1_vector[0] == 1);
    assert(*deduction_guide2_vector[0] == 1);
}

#else
    #pragma message("undefined __cpp_deduction_guides")
#endif

template <auto n>
struct B
{ /* ... */
};

B<5> b1; // OK: non-type template parameter type is int

B<'a'> b2;

// Error:
// B<2.5> b3;

tuple<int, int> foo_tuple()
{
    // return make_tuple(1, -1);
    return { 1, -1 };
}

template <auto... seq>
struct my_integer_sequence
{
};

/// @brief auto seq = integer_sequence<int, 0, 1, 2>();
auto seq = my_integer_sequence<0, 1, 2>();

// explicit constexpr
auto identity = [](int n) constexpr
{
    return n;
};
static_assert(identity(1) == 1);

// lambda with auto argument is actually a template
// implicit auto constexpr:
auto can_be_constexpr1 = [](auto a) { return a; };
auto can_be_constexpr2 = [](int (*fp)(int), auto a) { return fp(a); };

static_assert(can_be_constexpr2(can_be_constexpr1, 3) == 3);

// error: non-constant condition for static assertion
// static int i=0;
// static_assert(can_be_constexpr2(can_be_constexpr1, i) == 0);
auto non_const = [](auto a) {static int s; return a; };

constexpr int const_inc(int n)
{
    return [n] { return n + 1; }();
}

constexpr int (*inc)(int) = const_inc;
static_assert(const_inc(1) == 2);

void capture_this_by_value()
{
    struct capture_value_o
    {
        int value{ 1 };
        auto get_value_copy()
        {
            return [*this] { return value; };
        }
    };

    capture_value_o mo;
    auto val = mo.get_value_copy();
    mo.value = 2;
    assert(val() == 1);
}

void lambda_17()
{
    assert(can_be_constexpr1(2) == 2);
    int i = 3;
    assert(can_be_constexpr2(can_be_constexpr1, 3) == 3);
    non_const(1);
    capture_this_by_value();
}

template <typename... Args>
constexpr bool folding_and(Args... args)
{
    return (true && ... && args);
}

template <typename... Args>
constexpr auto folding_sum(Args... args)
{
    return (... + args);
}

void folding_demo()
{
    static_assert(!folding_and(true, false, true));
    static_assert(folding_sum(1.0, 2.0f, 3) == 6.0);
}

/// Before: namespace other { namespace other2 {
namespace outer_namespace::qualified_nested_namespace
{
int in_qualified_nested_namespace;
}

static_assert(__cpp_structured_bindings);

void structured_bindings()
{
    int a[2] = { 1, 2 };

    auto [a0, a1] = a;
    // a0_ref = &a; a0_ref = &a;
    auto& [a0_ref, a1_ref] = a;

    float x{};
    char y{};
    int z{};

    unordered_map<string, int> mapping{
        { "a", 1 },
        { "b", 2 },
        { "c", 3 },
    };

    for (const auto& [key, value] : mapping)
    {
        // Do something with key and value
    }
    //    tuple<float&,char&&,int> tpl(x,move(y),z);
    //    const auto& [a,b,c] = tpl;
}

void conditional_with_init()
{
    if (auto a = true)
    {
    };

    switch (int a = 10)
    {
    }
}

// Will warn if return of foo() is ignored
[[nodiscard]] int foo()
{
    return 0;
}
void test_attr()
{
    int a{ 1 };
    switch (a)
    {
        // Indicates that falling through on case 1 is intentional
    case 1:
        [[fallthrough]];
    case 2:
        // Indicates that b might be unused, such as on production builds
        [[maybe_unused]] int b = foo();
        assert(b > 0);
        break;
    }
}

void constif()
{
    if constexpr (true)
        printf("1\n");
}

static char char_u8 = u8'x';

enum byte_e : unsigned char
{
};
static byte_e b{ 123 };

void types_17()
{
    static_assert(is_integral_v<int>);
    static_assert(__cpp_hex_float);
    double hex_double = 0x1.2p3; // https://en.cppreference.com/w/cpp/language/floating_literal
    assert(hex_double == 9.0);
    static_assert(is_invocable<decltype(types_17)>::value);
    static_assert(is_invocable<int()>::value);
    static_assert(is_invocable_r<int, int()>::value);
    static_assert(is_invocable_r<void, void(int), int>::value);
    static_assert(negation_v<bool_constant<false>>);

    auto inc = [](int a) -> int { return a + 1; };
    static_assert(is_invocable_r<int, decltype(inc), int>::value);
    static_assert(__cpp_lib_invoke);
    // https://en.cppreference.com/w/cpp/utility/functional/invoke
    assert(invoke(inc, 2) == 3);
}

void map_demo()
{
    /// @brief https://en.cppreference.com/w/cpp/container/map/extract
    map<int, string> m{ { 1, "mango" }, { 2, "papaya" }, { 3, "guava" } };
    auto nh = m.extract(2);
    nh.key() = 4;
    m.insert(move(nh));
    // m == {{1, "mango"}, {3, "guava"}, {4, "papaya"}}
    set<int> src{ 1, 3, 5 };
    set<int> dst{ 2, 4, 5 };
    dst.merge(src);
}

void variant_demo()
{
    variant<int, float> v, w;
    v = 12; // v contains int
    int i = get<int>(v);
    w = get<int>(v);
    w = get<0>(v); // same effect as the previous line
    w = v;         // same effect as the previous line

    try
    {
        get<float>(w); // w contains int, not float: will throw
    }
    catch (const bad_variant_access&)
    {}

    using namespace literals;

    variant<string> x("abc");

    // converting constructors work when unambiguous
    x = "def"; // converting assignment also works when unambiguous

    variant<string, void const*> y("abc");

    // casts to void const * when passed a char const *
    assert(holds_alternative<void const*>(y));
    y = "xyz"s;
    assert(holds_alternative<string>(y));
}

void clamp_demo()
{
    static_assert(__cpp_lib_clamp);
    ///  clamp(x, low, high) == x < low ? low : x > high ? high : x;
    assert(clamp(0, 1, 3) == 1);
    assert(clamp(2, 1, 3) == 2);
    assert(clamp(4, 1, 3) == 3);
}

void dynamic_memory_17()
{
    int d[2] = { 10, 11 };
    unique_ptr<int[]> u1(d);
    assert(u1.get()[0] == 10); // C++11
    assert(u1[1] == 11);
    assert((bool)u1);
    u1.release();

    const shared_ptr<int> s1{ 0 };
    //    assert(typeid(reinterpret_pointer_cast<shared_ptr<int>>(s1)).name() == string("St10shared_ptrIS_IiEE"));
    vector<int> s = { 5, 7, 4, 2, 8, 6, 1, 9, 0, 3 };
    int a[] = { 1, 2, 3 };
    //    valarray va(a, 3); // uses explicit deduction guide
    //    static_assert(is_integral_v<remove_reference<decltype(va[0])>::type>);
}

void string_view_demo()
{
    static_assert(__cpp_lib_string_view);
    string s = "abcd";
    string_view v = s;
    assert(v.data() == s.c_str());
    assert(v.substr(1, 2).data() >= s.c_str());
    assert(v.substr(1, 2).data() <= s.c_str() + s.length());
}


inline int inline_var;



void cpp17Test()
{
    reference17();
    deduction_guides_17();
    folding_demo();
    lambda_17();
    variant_demo();
    clamp_demo();
    dynamic_memory_17();
    string_view_demo();
    // for (auto&& [first,second] : mymap) { }
    types_17();
    /// https://en.cppreference.com/w/cpp/language/range-for
    map<int, int> mymap;
    for (auto&& [first, second] : mymap) {
        // use first and second
    }
}