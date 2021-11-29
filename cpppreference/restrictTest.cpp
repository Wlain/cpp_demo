//
// Created by william on 2021/11/28.
//

int foo(int* a, int* b)
{
    *a = 5;
    *b = 6;
    return *a + *b;
}

int rfoo(int* __restrict a, int* __restrict b)
{
    *a = 5;
    *b = 6;
    return *a + *b;
}

void restrictTest()
{
    int* a = new int();
    int* b = new int();
    foo(a, b);
    rfoo(a, b);
    delete a;
    delete b;
    //# 生成64位Intel平台的代码：
    //foo:
    //    movl    $5, (%rdi)    # 存储 5 于 *a
    //    movl    $6, (%rsi)    # 存储 6 于 *b
    //    movl    (%rdi), %eax  # 从 *a 读回值，考虑到前面的存储会修改它
    //    addl    $6, %eax      # 将从 *a 读得的值加 6
    //    ret
    //
    //rfoo:
    //    movl      $11, %eax   # 结果是 11，编译时常量
    //    movl      $5, (%rdi)  # 存储 5 于 *a
    //    movl      $6, (%rsi)  # 存储 6 于 *b
    //    ret
}
