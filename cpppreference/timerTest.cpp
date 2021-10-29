//
// Created by william on 2021/10/29.
//

#include <chrono>
#include <thread>
#include <iostream>

void timerTest()
{
    auto begin = std::chrono::system_clock::now();
    // sleep 1 秒
    std::this_thread::sleep_for(std::chrono::seconds(1));
    // sleep 3 毫秒
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
    auto end = std::chrono::system_clock::now();
    std::cout << "duration: " << (end - begin).count() << "tick count" << std::endl;
    std::cout << "duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " milliseconds" << std::endl;
}