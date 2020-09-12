//
// Created by william on 2020/7/1.
//
#include "include/base.h"
#include <vector>

extern void testAuto();
extern void testDecltype();
extern void testConstantPointer();

class Vec2f {
public:
  Vec2f() = default;
  Vec2f(const Vec2f &) = default;
  virtual ~Vec2f() = default;
  Vec2f &operator=(const Vec2f &) = default;
  void setX(int x)
  {
    data[0] = x;
  }
  void setY(int y)
  {
    data[1] = y;
  }
private:
  int data[2] = {0, 0};
};

int main() {
  //  testAuto();
  //  testDecltype();
  //  testConstantPointer();
  std::shared_ptr<int> a = std::make_shared<int>();
  auto &temp = *a;
  temp = 1;
  std::shared_ptr<int> b = a;
  std::shared_ptr<int> c = std::make_shared<int>();
  auto &temp1 = *c;
  temp1 = 2;
  a.swap(c);
  std::shared_ptr<int> d = c;

  int test[] = {1, 2};
  test[0] = 0;
  test[1] = 1;
  int &t1 = test[1];
  int &t2 = t1;
  const Vec2f start = Vec2f();
  const Vec2f& dir = start;
  const Vec2f& dir_test = dir;
  Vec2f&& dir_test1 = Vec2f();
  Vec2f&& dir_test2 = Vec2f();

  std::vector<int> vec = {1, 3, 5, 7, 9};
  for (auto s = vec.size(); auto  item : vec)
  {
    std::cout << --s + item << ' ';
  }
  return 0;
}