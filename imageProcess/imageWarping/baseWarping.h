//
// Created by cwb on 2022/2/21.
//

#ifndef CPP_DEMO_BASEWARPING_H
#define CPP_DEMO_BASEWARPING_H
#include "vector2.h"

#include <QImage>
#include <vector>

class BaseWarping
{
public:
    BaseWarping();
    BaseWarping(std::vector<Vector2> p, std::vector<Vector2> q);
    virtual ~BaseWarping();
    virtual Vector2 targetFunction(const Vector2& input);
    void render(QImage& image, const QImage& m_originImage);
    void setPointP(const std::vector<Vector2>& mPointP);
    void setPointQ(const std::vector<Vector2>& mPointQ);
    void resize(uint32_t width, uint32_t height);
    inline uint32_t width() const { return m_width; }
    inline uint32_t height() const { return m_height; }
    int getFrameBufferIndex(int i, int j) const;
    bool getFilledStatusAt(int i, int j) const;
    void setFilledStatusAt(int i, int j, bool status);
    void resetFilledStatus();

protected:
    /// 方案一：直接搞个boxBlur，会有边缘case
    void fillNearPixelForBoxBlur(QImage& image) const;
    /// 方案二：使用ANN进行搜索最邻近点，效果更好
    void fillNearPixelForANNSearch(QImage& image);

protected:
    std::vector<bool> m_filled;
    std::vector<Vector2> m_pointP, m_pointQ;
    float m_exponent = 2.0f;
    size_t m_pointSize = 0;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
};

#endif //CPP_DEMO_BASEWARPING_H
