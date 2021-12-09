//
// Created by william on 2021/12/9.
//

#include "geekTimeCpp/common/shape.h"

namespace geekTimeTest
{
class ShapeWrapper
{
public:
    explicit ShapeWrapper(Shape* ptr = nullptr) :
        m_ptr(ptr)
    {}
    ~ShapeWrapper()
    {
        delete m_ptr;
    }
    Shape* get() const { return m_ptr; }

private:
    Shape* m_ptr = nullptr;
};

void foo()
{
    ShapeWrapper shapeWrapper(createShape(ShapeType::Circle));
}
void shapeWrapTest()
{
    foo();
}
} // namespace geekTimeTest