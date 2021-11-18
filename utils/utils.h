//
// Created by ytech on 2021/10/25.
//

#ifndef CPP_DEMO_UTILS_H
#define CPP_DEMO_UTILS_H

#include <sys/stat.h>

inline bool isFileExist(const std::string& name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

#endif //CPP_DEMO_UTILS_H
