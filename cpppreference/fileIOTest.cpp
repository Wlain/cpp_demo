//
// Created by william on 2021/7/17.
//


#include "base.h"

namespace fs = std::filesystem;

void fileIOTest()
{
    fs::path p{ "/Users/william/Desktop" };
    p += "/test.txt";
    bool b1 = fs::is_directory(p);
    std::cout << std::boolalpha << "p is directory:" << b1 << std::endl;
    fs::remove_all(p);
    std::cout << p.relative_path() << std::endl;
}