//
// Created by william on 2020/11/13.
//

#include "base.h"

// 函数指针
template <typename T>
int add(T a, T b) { return a + b; }

void callbackTest() {
  int (*p)(int, int);
  p = add;
  int a = (*p)(1, 2);
  int b = p(2, 2);
  std::cout << "a:" << a << std::endl;
  std::cout << "b:" << b << std::endl;
}