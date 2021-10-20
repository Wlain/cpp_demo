//
// Created by william on 2021/5/4.
//
#include <iostream>
#include <memory>
namespace enableSharedFromThisTest
{
struct Good : std::enable_shared_from_this<Good> // note: public inheritance
{
    // returns a shared_ptr which shares ownership of *this
    std::shared_ptr<Good> getPtr()
    {
        return shared_from_this();
    }
    // returns the weak_ptr which shares ownership of *this
    std::weak_ptr<Good> getWeakPtr()
    {
        return weak_from_this();
    }
    // operator=, returns a reference to this

    ~Good()
    {
        std::cout << "ThreadTest::ThreadTestadTest() called\n";
    }
};

struct Bad
{
    std::shared_ptr<Bad> getPtr()
    {
        return std::shared_ptr<Bad>(this);
    }
    ~Bad()
    {
        std::cout << "Bad::~Bad() called\n";
    }
};

class Foo : public std::enable_shared_from_this<Foo>
{
public:
    Foo()
    {
        std::cout << "Foo::Foo() called\n";
    }
    ~Foo()
    {
        std::cout << "Foo::~Foo() called\n";
    }

    std::shared_ptr<Foo> getPtr()
    {
        return shared_from_this();
    }
};

void enableSharedFromThisTest()
{
    Foo* f = new Foo();
    std::cout << f << std::endl;
    std::shared_ptr<Foo> f1;
    {
        std::shared_ptr<Foo> f2(f);
        std::cout << f2.get() << std::endl;
        f1 = f2->getPtr();
    }
    std::cout << "pf2 is gone\n";
    std::cout << f1.get() << std::endl;
//    // ThreadTest the two shared_ptr's share the same object
//    std::shared_ptr<ThreadTest> gp1 = std::make_shared<ThreadTest>();
//    std::shared_ptr<ThreadTest> gp2 = gp1->getPtr();
//    std::cout << "gp2.use_count() = " << gp1.use_count() << '\n';
//    //     Bad: shared_from_this is called without having std::shared_ptr owning the caller
//    try
//    {
//        ThreadTest not_so_good;
//        std::shared_ptr<ThreadTest> gp3 = not_so_good.getPtr();
//    }
//    catch (std::bad_weak_ptr& e)
//    {
//        // undefined behavior (until C++17) and std::bad_weak_ptr thrown (since C++17)
//        std::cout << e.what() << '\n';
//    }
//    // Bad, each shared_ptr thinks it's the only owner of the object
//    std::shared_ptr<Bad> bp1 = std::make_shared<Bad>();
//    std::shared_ptr<Bad> bp2 = bp1->getPtr();
//    std::cout << "bp2.use_count() = " << bp2.use_count() << '\n';
}

} // namespace enableSharedFromThisTest
