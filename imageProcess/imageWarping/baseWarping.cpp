//
// Created by cwb on 2022/2/21.
//

#include "baseWarping.h"

#include "vec3.h"

#include <ANN/ANN.h>

BaseWarping::BaseWarping() = default;

BaseWarping::~BaseWarping() = default;

BaseWarping::BaseWarping(std::vector<Vector2> p, std::vector<Vector2> q) :
    m_pointP(std::move(p)), m_pointQ(std::move(q))
{
    m_pointSize = m_pointP.size();
}

Vector2 BaseWarping::targetFunction(const Vector2& input)
{
    return {};
}

void BaseWarping::setPointP(const std::vector<Vector2>& mPointP)
{
    m_pointP = mPointP;
}
void BaseWarping::setPointQ(const std::vector<Vector2>& mPointQ)
{
    m_pointQ = mPointQ;
}

void BaseWarping::resize(uint32_t width, uint32_t height)
{
    m_filled.resize(width * height);
    m_width = width;
    m_height = height;
}

int BaseWarping::getFrameBufferIndex(int y, int x) const
{
    assert(x >= 0 && x < m_width && y >= 0 && y < m_height);
    return (m_height - y) * m_width + x;
}

bool BaseWarping::getFilledStatusAt(int y, int x) const
{
    return m_filled[getFrameBufferIndex(y, x)];
}

void BaseWarping::setFilledStatusAt(int y, int x, bool status)
{
    m_filled[getFrameBufferIndex(y, x)] = status;
}

void BaseWarping::resetFilledStatus()
{
    std::fill(m_filled.begin(), m_filled.end(), false);
}

void BaseWarping::fillNearPixelForBoxBlur(cv::Mat& image) const
{
    for (int i = 1; i < m_height - 1; ++i)
    {
        for (int j = 1; j < m_width - 1; ++j)
        {
            if (!getFilledStatusAt(i, j))
            {
                Vec3i sum{ 0, 0, 0 };
                auto index = 0;
                // 如果像素周边有像素
                if (getFilledStatusAt(i - 1, j - 1) || getFilledStatusAt(i, j - 1) || getFilledStatusAt(i + 1, j - 1) ||
                    getFilledStatusAt(i - 1, j) || getFilledStatusAt(i + 1, j) ||
                    getFilledStatusAt(i - 1, j + 1) || getFilledStatusAt(i, j + 1) || getFilledStatusAt(i + 1, j + 1))
                {
                    for (int n = i - 1; n <= i + 1; ++n)
                    {
                        for (int m = j - 1; m <= j + 1; ++m)
                        {
                            if (n == i && m == j) break;
                            if (getFilledStatusAt(n, m))
                            {
                                auto color = image.at<cv::Vec3b>(n, m); //image.pixelColor();
                                sum += Vec3i(color[0], color[1], color[2]);
                                index++;
                            }
                        }
                    }
                }
                if (!sum.isZero())
                {
                    const auto& avg = sum / index;
                    image.at<cv::Vec3b>(i, j) = { (unsigned char)avg.x, (unsigned char)avg.y, (unsigned char)avg.z };
                }
            }
        }
    }
}

void BaseWarping::fillNearPixelForANNSearch(cv::Mat& image)
{
    int nPts = 0;
    int count = 4;
    int threshold = 2;
    ANNpointArray dataPts = annAllocPts(m_width * m_height, 2);
    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            if (getFilledStatusAt(i, j))
            {
                dataPts[nPts][0] = i;
                dataPts[nPts][1] = j;
                nPts++;
            }
        }
    }
    auto kdTree = ANNkd_tree(dataPts, nPts, 2);
    std::vector<ANNidx> index(count);
    std::vector<ANNdist> dist(count);
    for (int i = 0; i < m_height; ++i)
    {
        for (int j = 0; j < m_width; ++j)
        {
            if (!getFilledStatusAt(i, j))
            {
                Vec3i sum{ 0, 0, 0 };
                ANNpoint pt = annAllocPt(2);
                pt[0] = i;
                pt[1] = j;
                kdTree.annkSearch(pt, count, index.data(), dist.data(), 0);
                int colorIndex = 0;
                for (int m = 0; m < count; m++)
                {
                    /// 注意，此处是opencv，宽高颠倒，y是取 dataPts[index[m]][0]
                    auto y = dataPts[index[m]][0];
                    auto x = dataPts[index[m]][1];
                    auto color = image.at<cv::Vec3b>(y, x);
                    if (fabs(y - i) <= threshold && fabs(x - j) <= threshold)
                    {
                        sum += Vec3i(color[0], color[1], color[2]);
                        colorIndex++;
                    }
                }
                if (!sum.isZero())
                {
                    const auto& avg = sum / colorIndex;
                    image.at<cv::Vec3b>(i, j) = { (unsigned char)avg.x, (unsigned char)avg.y, (unsigned char)avg.z };
                }
            }
        }
    }
}

void BaseWarping::getWarpingResult(const cv::Mat& img, cv::Mat& result)
{
    // each y, x
    for (int x = 0; x < m_height; ++x)
    {
        for (int y = 0; y < m_width; ++y)
        {
            Vector2 point{ (float)x, (float)y };
            point = targetFunction(point);
            auto resultX = (int)point.x;
            auto resultY = (int)point.y;
            if (resultX >= 0 && resultX < m_width && resultY >= 0 && resultY < m_height)
            {
                setFilledStatusAt(resultY, resultX, true);
                result.at<cv::Vec3b>(resultY, resultX) = img.at<cv::Vec3b>(y, x);
            }
        }
    }
    fillNearPixelForANNSearch(result);
}
