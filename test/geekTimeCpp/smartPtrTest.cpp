//
// Created by william on 2021/12/9.
//

#ifndef CPP_DEMO_SMARTPTRTEST_CPP
#define CPP_DEMO_SMARTPTRTEST_CPP
#include "geekTimeCpp/common/shape.h"

#include <utility> // std::swap/move

namespace geekTimeTest
{
template <typename T>
class SmartPtr
{
public:
    explicit SmartPtr(T* ptr = nullptr) :
        m_ptr(ptr) {}
    ~SmartPtr()
    {
        delete m_ptr;
    }

    SmartPtr(SmartPtr&& other)
    {
        m_ptr = other.release();
    }

    SmartPtr& operator=(SmartPtr rhs)
    {
        rhs.swap(*this);
        return *this;
    }

    T* get() const { return m_ptr; }
    T& operator*() const { return *m_ptr; }
    T* operator->() { return m_ptr; }
    explicit operator bool() const { return m_ptr; }
    T* release()
    {
        T* ptr = m_ptr;
        m_ptr = nullptr;
        return ptr;
    }

    void swap(SmartPtr& rhs)
    {
        std::swap(m_ptr, rhs.m_ptr);
    }

private:
    T* m_ptr;
};

void smartPtrTest()
{
    SmartPtr<Shape> ptr1(createShape(ShapeType::Circle));
    // SmartPtr<Shape> ptr2(ptr1);  // Cannot compile
    SmartPtr<Shape> ptr3;
    // ptr3 = ptr1;                             // Cannot compile
    ptr3 = std::move(ptr1);                  // OK
    SmartPtr<Shape> ptr4{ std::move(ptr3) }; // OK
}

} // namespace geekTimeTest

#endif //CPP_DEMO_SMARTPTRTEST_CPP
