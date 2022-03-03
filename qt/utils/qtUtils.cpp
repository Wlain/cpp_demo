//
// Created by william on 2022/2/26.
//

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