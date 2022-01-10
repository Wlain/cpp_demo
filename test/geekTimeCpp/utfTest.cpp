//
// Created by william on 2022/1/10.
//
#include <iomanip>   // std::setw
#include <iostream>  // std::cout/endl/hex
#include <stdexcept> // std::runtime_error
#include <string>    // srd::string/u16string

using namespace std;

namespace geekTimeTest
{
const char32_t unicodeMax = 0x10FFFF;

void toUtf16(char32_t ch, u16string& result)
{
    if (ch > unicodeMax)
    {
        throw std::runtime_error("invalid code point");
    }
    if (ch < 0x10000)
    {
        result += char16_t(ch);
    }
    else
    {
        char16_t first = 0xD800 | ((ch - 0x10000) >> 10);
        char16_t second = 0xDC00 | (ch & 0x3FF);
        result += first;
        result += second;
    }
}

void toUtf8(char32_t ch, string& result)
{
    if (ch > unicodeMax)
    {
        throw std::runtime_error("invalid code point");
    }
    if (ch < 0x80)
    {
        result += ch;
    }
    else if (ch < 0x10000)
    {
        result += 0xE0 | (ch >> 12);
        result += 0x80 | ((ch >> 6) & 0x3F);
        result += 0x80 | (ch & 0x3F);
    }
    else
    {
        result += 0xF0 | (ch >> 18);
        result += 0x80 | ((ch >> 12) & 0x3F);
        result += 0x80 | ((ch >> 6) & 0x3F);
        result += 0x80 | (ch & 0x3F);
    }
}

void utfTest()
{
    char32_t str[] = U" \u6C49\U0001F600";
    u16string u16Str;
    string u8Str;
    for (auto ch : str)
    {
        if (ch == 0)
        {
            break;
        }
        toUtf16(ch, u16Str);
        toUtf8(ch, u8Str);
    }
    cout << hex << setfill('0');
    for (char16_t ch : u16Str)
    {
        cout << setw(4) << unsigned(ch) << ' ';
    }
    cout << endl;
    for (unsigned char ch : u8Str)
    {
        cout << setw(2) << unsigned(ch) << ' ';
    }
    cout << endl;
    cout << u8Str << endl;
}
} // namespace geekTimeTest