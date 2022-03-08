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

int BaseWarping::getFrameBufferIndex(int i, int j) const
{
    assert(i >= 0 && i < m_width && j >= 0 && j < m_height);
    return j * m_width + i;
}

bool BaseWarping::getFilledStatusAt(int i, int j) const
{
    return m_filled[getFrameBufferIndex(i, j)];
}

void BaseWarping::setFilledStatusAt(int i, int j, bool status)
{
    m_filled[getFrameBufferIndex(i, j)] = status;
}

void BaseWarping::resetFilledStatus()
{
    std::fill(m_filled.begin(), m_filled.end(), false);
}

void BaseWarping::fillNearPixelForBoxBlur(QImage& image) const
{
    for (int i = 1; i < m_width - 1; ++i)
    {
        for (int j = 1; j < m_height - 1; ++j)
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
                                auto color = image.pixelColor(n, m);
                                sum += Vec3i(color.red(), color.green(), color.blue());
                                index++;
                            }
                        }
                    }
                }
                if (!sum.isZero())
                {
                    const auto& avg = sum / index;
                    image.setPixel(i, j, qRgba(avg.x, avg.y, avg.z, 255));
                }
            }
        }
    }
}

void BaseWarping::fillNearPixelForANNSearch(QImage& image)
{
    int nPts = 0;
    int count = 4;
    int threshold = 2;
    ANNpointArray dataPts = annAllocPts(m_width * m_height, 2);
    for (int i = 0; i < m_width; i++)
    {
        for (int j = 0; j < m_height; j++)
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
    for (int i = 0; i < m_width; ++i)
    {
        for (int j = 0; j < m_height; ++j)
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
                    auto x = dataPts[index[m]][0];
                    auto y = dataPts[index[m]][1];
                    auto color = image.pixelColor(x, y);
                    if (fabs(x - i) <= threshold && fabs(y - j) <= threshold)
                    {
                        sum += Vec3i(color.red(), color.green(), color.blue());
                        colorIndex++;
                    }
                }
                if (!sum.isZero())
                {
                    const auto& avg = sum / colorIndex;
                    image.setPixel(i, j, qRgba(avg.x, avg.y, avg.z, 255));
                }
            }
        }
    }
}

void BaseWarping::render(QImage& image, const QImage& m_originImage)
{
    for (int i = 0; i < m_width; ++i)
    {
        for (int j = 0; j < m_height; ++j)
        {
            Vector2 point{ (float)i, (float)j };
            point = targetFunction(point);
            if (point.x >= 0.0f && point.x < (float)m_width && point.y >= 0.0f && point.y < (float)m_height)
            {
                setFilledStatusAt((int)point.x, (int)point.y, true);
                image.setPixel((int)point.x, (int)point.y, m_originImage.pixel(i, j));
            }
        }
    }
    fillNearPixelForANNSearch(image);
}
