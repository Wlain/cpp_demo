//
// Created by william on 2021/8/2.
//
/// 发送 sig 给程序。调用以 signal() 指定的信号处理函数。

#include "base.h"

void signalHandler(int signal)
{
    printf("Received signal %d\n", signal);
}

void raiseTest()
{
    /// 安装信号处理函数。
    signal(SIGTERM, signalHandler);
    printf("Sending signal %d\n", SIGTERM);
    raise(SIGTERM);
    printf("Exit raiseTest()\n");
}