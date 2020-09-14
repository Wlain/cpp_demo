//
// Created by william on 2020/7/1.
//
#include "include/base.h"
#include "vec2f.h"
#include <vector>

extern void testAuto();
extern void testDecltype();
extern void testConstantPointer();

int Vec2f::constructorCount = 0;
int Vec2f::destructorCount = 0;
// 必须在定义类的文件中对静态成员变量进行一次说明,或初始化。否则编译能通过，链接不能通过。

int main()
{
    //  testAuto();
    //  testDecltype();
    //  testConstantPointer();
    //  std::shared_ptr<int> a = std::make_shared<int>();
    //  auto &temp = *a;
    //  temp = 1;
    //  std::shared_ptr<int> b = a;
    //  std::shared_ptr<int> c = std::make_shared<int>();
    //  auto &temp1 = *c;
    //  temp1 = 2;
    //  a.swap(c);
    //  std::shared_ptr<int> d = c;
    //
    //  int test[] = {1, 2};
    //  test[0] = 0;
    //  test[1] = 1;
    //  int &t1 = test[1];
    //  int &t2 = t1;
    //  const Vec2f start = Vec2f();
    //  const Vec2f& dir = start;
    //  const Vec2f& dir_test = dir;
    //  Vec2f&& dir_test1 = Vec2f();
    //  Vec2f&& dir_test2 = Vec2f();

    //  std::vector<int> vec = {1, 3, 5, 7, 9};
    //  for (auto s = vec.size(); auto  item : vec)
    //  {
    //    std::cout << --s + item << ' ';
    //  }
    Vec2f *points = new Vec2f[10];
    for (int i = 0; i < 10; ++i)
    {
        points[i].setX(i);
        points[i].setY(10 * i);
    }
    //  Vec2f leftEye = points[1];
    //  Vec2f rightEye = points[2];
    const Vec2f &leftEye1 = points[1] - points[2];
    std::cout << "constructorCount:" << Vec2f::getConstructorCount() << std::endl;
    std::cout << "destructorCount:" << Vec2f::getDestructorCount() << std::endl;
    std::cout << "=================================" << std::endl;
    delete[] points;
    std::cout << "constructorCount:" << Vec2f::getConstructorCount() << std::endl;
    std::cout << "destructorCount:" << Vec2f::getDestructorCount() << std::endl;
    return 0;
}