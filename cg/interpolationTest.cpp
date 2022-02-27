//
// Created by william on 2021/11/30.
//

#include "cgCommonDefine.h"

#include <fstream>
#include <random>
namespace cg
{
static std::random_device r;
// 选择 1 与 6 间的随机数
static std::default_random_engine e1(r());
static std::uniform_int_distribution<int> uniformDist(0, 255);

auto random()
{
    return uniformDist(e1);
}

template <typename T>
T bilinear(const float& tx, const float& ty, const T c00, const T c10, const T c01, const T c11)
{
    T a = c00 * (1.0f - tx) + c10 * tx;
    T b = c01 * (1.0f - tx) + c11 * tx;
    return (a * (1.0f - ty) + b * ty) / 255.0f;
}

void saveToPPM(const char* fn, const Color3f* c, const int& width, const int& height)
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
        const Color3f* pc = c;
        for (int j = 0; j < height; ++j)
        {
            for (int i = 0; i < width; ++i)
            {
                char r = static_cast<char>(std::min(255.f, pc->m_red));
                char g = static_cast<char>(std::min(255.f, pc->m_green));
                char b = static_cast<char>(std::min(255.f, pc->m_blue));
                ofs << r << g << b;
                pc++;
            }
        }
    }
    ofs.close();
}

/// 双线性插值测试
void bilinearInterpolationTest()
{
    int imageWidth = 512;
    int gridSizeX = 9, gridSizeY = 9;
    auto* grid2d = new Color3f[(gridSizeX + 1) * (gridSizeY + 1)]; // 网格
    for (int j = 0, k = 0; j <= gridSizeY; ++j)
    {
        for (int i = 0; i <= gridSizeX; ++i, ++k)
        {
            grid2d[j * (gridSizeX + 1) + i] = { (float)random(), (float)random(), (float)random() };
        }
    }
    std::vector<Color3f> imageData;
    imageData.reserve(imageWidth * imageWidth);
    for (int j = 0; j < imageWidth; ++j)
    {
        for (int i = 0; i < imageWidth; ++i)
        {
            float gx = i / float(imageWidth) * gridSizeX;
            float gy = j / float(imageWidth) * gridSizeY;
            int gxi = int(gx);
            int gyi = int(gy);
            const Color3f& c00 = grid2d[gyi * (gridSizeX + 1) + gxi];
            const Color3f& c10 = grid2d[gyi * (gridSizeX + 1) + (gxi + 1)];
            const Color3f& c01 = grid2d[(gyi + 1) * (gridSizeX + 1) + gxi];
            const Color3f& c11 = grid2d[(gyi + 1) * (gridSizeX + 1) + (gxi + 1)];
            imageData.push_back(bilinear(gx - gxi, gy - gyi, c00, c10, c01, c11));
        }
    }
    cv::Mat image(imageWidth, imageWidth, CV_32FC3, imageData.data());
    cv::imshow("bilinearInterpolationTest", image);
    cv::waitKey();
}

/// 三线性插值测试
void trilinearInterpolationTest()
{
    auto* imageData = new Color3f[width * height];
    display("trilinearInterpolationTest", imageData);
    delete[] imageData;
}
} // namespace cg