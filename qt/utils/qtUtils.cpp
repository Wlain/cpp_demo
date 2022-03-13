//
// Created by william on 2022/2/26.
//

#include "vector2.h"

#include <QImage>
#include <opencv2/opencv.hpp>

//Qt读入彩色图后一般为Format_RGB32格式（4通道），而OpenCV一般用3通道的，因此进行了转换。
cv::Mat qImage2Mat(const QImage& image)
{
    cv::Mat mat;
    switch (image.format())
    {
    case QImage::Format_RGB32: //一般Qt读入彩色图后为此格式
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR); //转3通道
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}

QImage mat2Qimage(const cv::Mat& mat)
{
    if (mat.type() == CV_8UC1 || mat.type() == CV_8U)
    {
        QImage image((const uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
        return image;
    }
    else if (mat.type() == CV_8UC3)
    {
        QImage image((const uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped(); //r与b调换
    }
}

bool imageValid(std::unique_ptr<QImage>& image)
{
    return image != nullptr && !image->isNull();
}

void cropImage(cv::Mat& image)
{
    auto width = image.cols;
    auto height = image.rows;
    if (height > width)
    {
        auto temp = image(cv::Rect(0, (height -width) / 2, width, width));
        image = temp;
    }
    else
    {
        auto temp = image(cv::Rect((width - height) / 2, 0, height, height));
        image = temp;
    }
}

// Neighbor offsets in all directions
static const int offsets[5][2] = { { 0, 0 }, { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 } };

/// 直线扫描算法
/// 1.DDA画线算法（数值微分法）：引进图形学中一个很重要的思想—增量思想。
/// 点xi，yi满足直线方程yi=kxi+b，
/// 若xi增加一个单位，则下一步点的位置（xi+1，yi+1）满足yi+1 = k（xi + 1）+ b。
/// 即yi+1 = yi + k。
void ddaLine(const Vector2& begin, const Vector2& end, QImage& image)
{
    auto x1 = begin.x;
    auto y1 = begin.y;

    auto x2 = end.x;
    auto y2 = end.y;
    auto color = qRgb(0, 0, 0);
    int dx = (int)x2 - (int)x1;
    int dy = (int)y2 - (int)y1;
    float k = (float)dy / (float)dx;
    float _k = (float)dx / (float)dy;
    int absDx = fabs(dx);
    int absDy = fabs(dy);
    if (absDx >= absDy) //  // 斜率 k <= 1 || 斜率 k >= -1
    {
        // 如果第二个点在第一个点的左边，需要做一个交换
        if (dx < 0)
        {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        while (x1 < x2)
        {
            /// 改良DDA，主要用于加粗直线
            for(auto offset : offsets)
            {
                image.setPixel(x1 + offset[0], int(y1 + 0.5f) + offset[1], color);
            }
            y1 = y1 + k;
            x1 = x1 + 1;
        }
    }
    else // 斜率k > 1 || 斜率 k < -1
    {
        if (dy < 0)
        {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        while (y1 < y2)
        {
            /// 改良DDA，主要用于加粗直线
            for(auto offset : offsets)
            {
                image.setPixel(int(x1 + 0.5f) + offset[0], y1 + offset[1], color);
            }
            x1 = x1 + _k;
            y1 = y1 + 1;
        }
    }
}