//
// Created by william on 2022/3/2.
//

#ifndef CPP_DEMO_IMAGEWIDGET_H
#define CPP_DEMO_IMAGEWIDGET_H

#include "vector2.h"

#include <QWidget>
#include <array>
#include <opencv2/opencv.hpp>
#include <primitive/shape.h>
#include <vector>

QT_BEGIN_NAMESPACE
class QImage;
QT_END_NAMESPACE
class BaseWarping;

class ImageWidget : public QWidget
{
public:
    struct ImageWrapper
    {
        std::unique_ptr<QImage> qImage;
        std::unique_ptr<cv::Mat> matImage;
        cv::Rect rect;
        bool isReady = false;
    };

public:
    Q_OBJECT

public:
    ImageWidget();
    ~ImageWidget() override;

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void paintEvent(QPaintEvent* paintEvent) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

public slots:
    void actionNew();
    void actionOpen();
    void actionSave();
    void actionClose();
    void actionUndo();
    void actionInvert();
    void actionGray();
    void actionIDW();
    void actionRBF();
    void actionMirrorH();
    void actionMirrorV();
    void actionOrigin();
    void actionOpenSourceImage();
    void actionOpenTargetImage();
    void actionOpenMask();
    void actionAlphaBlend();
    void actionPossionImageBlend();
    void actionColorTransform();
    void actionLine();
    void actionRect();
    void actionPen();
    void actionOval();
    void actionSetWidth();
    void actionPalette();
    void actionAbout();
    void actionHelp();

private:
    template <class Warping>
    void renderWarping();

private:
    void destroy();
    void calcPressPoint(QMouseEvent* event);
    void openImage(ImageWrapper& image);

private:
    std::unique_ptr<cv::Mat> m_matOriginImage;
    std::unique_ptr<QImage> m_image;
    std::unique_ptr<QImage> m_originImage;
    ImageWrapper m_sourceImage;
    ImageWrapper m_targetImage;
    ImageWrapper m_maskImage;
    ImageWrapper m_resultImage;
    std::unique_ptr<QImage> m_textImage;
    std::unique_ptr<Shape> m_shape;
    std::unique_ptr<BaseWarping> m_warping;
    std::vector<std::unique_ptr<Shape>> m_shapeList;
    Shape::Primitive m_primitiveType = Shape::Primitive::None;
    std::vector<Vector2> m_starts, m_ends;
    std::string m_iconText;
    Vector2 m_top;
    Vector2 m_pressPoint;
    QPen m_pen;
    QPainter m_painter;
    QPainter m_primitivePainter;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    Vector2 m_lastEditPos;
    bool m_isFirstDrawEditPos = true;
    bool m_drawStatus = false;
    constexpr static float const s_imageWidth = 300;
    const std::vector<std::vector<int>> m_offsets = { { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 } };
};

#endif //CPP_DEMO_IMAGEWIDGET_H
