//
// Created by william on 2021/12/9.
//

#include "geekTimeCpp/common/shape.h"
#include "geekTimeCpp/common/shartPtr.h"

#include <cstdio>
#include <memory>
namespace geekTimeTest
{
void sharedPtrTest()
{
    SharedPtr<Circle> ptr1(new Circle());
    printf("use count of ptr1 is %ld\n", ptr1.useCount());
    SharedPtr<Shape> ptr2(new Shape());
    printf("use count of ptr2 was %ld\n", ptr2.useCount());
//    ptr2 = ptr1;
    printf("use count of ptr2 was %ld\n", ptr2.useCount());
    if (ptr1)
    {
        puts("ptr1 is not empty");
    }
    SharedPtr<Circle> ptr3 = dynamicPointerCast<Circle>(ptr2);
    printf("use count of ptr3 is %ld\n", ptr3.useCount());
}
} // namespace geekTimeTest
