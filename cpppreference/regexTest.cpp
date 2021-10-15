//
// Created by william on 2021/6/11.
//
#include "base.h"

void regexTest()
{
    std::string text = "Quick brown fox";
    std::regex vowel_re("fox");
    std::cout << '\n'
              << std::regex_replace(text, vowel_re, "hello") << '\n';
}