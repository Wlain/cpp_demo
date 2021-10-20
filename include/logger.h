//
// Created by william on 2021/10/12.
//

#ifndef FM_BACKEND_WASM_LOGGER_H
#define FM_BACKEND_WASM_LOGGER_H
#include <cstdio>
#include <cstdlib>
#include <iostream>

class Logger
{
public:
    enum class Level : uint32_t
    {
        Info = 0,
        Warn,
        Error
    };

public:
    static void log(Level level, const char* message, ...);
};

#endif //FM_BACKEND_WASM_LOGGER_H
