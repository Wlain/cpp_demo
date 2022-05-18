//
// Created by william on 2022/5/17.
//
#include "commonMacro.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <random>


std::wstring DisplayPathInfo()
{
    using namespace std::filesystem;
    using std::endl;
    path pathToDisplay(GET_CURRENT("cpppreference/fileIOTest.cpp"));
    std::wostringstream wos;
    int i = 0;
    wos << L"Displaying path info for: " << pathToDisplay << std::endl;
    for (const auto& p : pathToDisplay)
    {
        wos << L"path part: " << i++ << L" = " << p << std::endl;
    }
    wos << L"root_name() = " << pathToDisplay.root_name() << endl
        << L"root_path() = " << pathToDisplay.root_path() << endl
        << L"relative_path() = " << pathToDisplay.relative_path() << endl
        << L"parent_path() = " << pathToDisplay.parent_path() << endl
        << L"filename() = " << pathToDisplay.filename() << endl
        << L"stem() = " << pathToDisplay.stem() << endl
        << L"extension() = " << pathToDisplay.extension() << endl;
    return wos.str();
}

void readFileFromPath(std::filesystem::path& path)
{
    // Open, read, and close the file.
    std::wifstream readFile;
    std::wstring line;
    readFile.open(path); // implicit conversions
    std::wcout << L"File " << path << L" contains:" << std::endl;
    while (readFile.good())
    {
        getline(readFile, line);
        std::wcout << line << std::endl;
    }
    readFile.close();
}

void writeFileForPath(std::filesystem::path& path, std::string_view string)
{
    // Open, write to, and close the file.
    std::wofstream writeFile(path, std::ios::out); // implicit conversion
    writeFile << string.data();
    writeFile.close();
}

void saveToPPM(const char* fn, std::vector<unsigned int>& color, const int& width, const int& height)
{
    std::ofstream ofs;
    // flags are necessary if your compile on Windows
    ofs.open(fn, std::ios::out | std::ios::binary);
    if (ofs.fail())
    {
        fprintf(stderr, "ERROR: can't save image to file %s\n", fn);
    }
    else
    {
        ofs << "P6\n"
            << width << " " << height << "\n255\n";
        auto* ptr = color.data();
        for (int j = 0; j < height; ++j)
        {
            for (int i = 0; i < width; ++i)
            {
                char r = static_cast<char>(ptr[0]);
                char g = static_cast<char>(ptr[1]);
                char b = static_cast<char>(ptr[2]);
                ofs << r << g << b;
                ptr += 3;
            }
        }
    }
    ofs.close();
}

void filesystemTest()
{
    std::wcout << DisplayPathInfo() << std::endl;
    std::filesystem::path filePath(L"/Users/william/git/sample/cpp_demo/resources");
    filePath /= L"writeTest.txt";
    writeFileForPath(filePath, "Lorem ipsum\nDolor sit amet\"");
    auto writePath = filePath.parent_path() / "readTest.txt";
    readFileFromPath(writePath);
    const auto width = 720;
    const auto height = 1280;
    std::vector<unsigned int> image(3 * width * height);
    // Set up random number distribution
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<int> dist(0, 255);
    std::generate(image.begin(), image.end(), [&]() { return dist(engine); });
    saveToPPM("/Users/william/git/sample/cpp_demo/resources/save.ppm", image, width, height);
}