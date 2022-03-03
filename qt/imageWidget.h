//
// Created by william on 2022/3/2.
//

#ifndef CPP_DEMO_IMAGEWIDGET_H
#define CPP_DEMO_IMAGEWIDGET_H

#include "vector2.h"

#include <QWidget>
#include <primitive/shape.h>
#include <vector>
#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
class QImage;
QT_END_NAMESPACE
class BaseWarping;

class ImageWidget : public QWidget
{
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
    void actionColorTransform();
    void actionLine();
    void actionRect();
    void actionAbout();
    void actionHelp();

private:
    void destroy();

private:
    std::unique_ptr<cv::Mat> m_matOriginImage;
    std::unique_ptr<QImage> m_image;
    std::unique_ptr<QImage> m_originImage;
    std::unique_ptr<Shape> m_shape;
    std::unique_ptr<BaseWarping> m_warping;
    std::vector<std::unique_ptr<Shape>> m_shapeList;
    Shape::Primitive m_primitiveType = Shape::Primitive::Line;
    std::vector<Vector2> m_starts, m_ends;
    QPen m_pen;
    QPainter m_painter;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    bool m_drawStatus = false;
};

#endif //CPP_DEMO_IMAGEWIDGET_H
