//
// Created by william on 2022/3/2.
//

#include "imageWidget.h"

#include "imageWarping/idwWarping.h"
#include "imageWarping/rbfWarping.h"
#include "primitive/line.h"
#include "primitive/rect.h"

#include <QFileDialog>
#include <QKeyEvent>
#include <QPainter>
#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>

extern cv::Mat qImage2Mat(const QImage& image);
extern QImage mat2Qimage(const cv::Mat& mat);
extern cv::Mat colorTransferBetweenImages();
extern cv::Mat channelSwap(const cv::Mat& img);
extern cv::Mat grayTest(const cv::Mat& img);
extern cv::Mat mirrorTest(const cv::Mat& img, bool horizontal, bool vertical);

ImageWidget::ImageWidget()
{
    m_image = std::make_unique<QImage>();
    m_originImage = std::make_unique<QImage>();
    m_pen = QPen(QColor("red"), 2);
}

ImageWidget::~ImageWidget() = default;

void ImageWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Space)
    {
        actionOpen();
    }
}

void ImageWidget::paintEvent(QPaintEvent* paintEvent)
{
    m_painter.begin(this);
    // clear background
    m_painter.setBrush(Qt::white);
    QRect back_rect(0, 0, width(), height());
    m_painter.drawRect(back_rect);
    // render image
    if (m_image != nullptr)
    {
        m_top.set((width() - m_image->width()) / 2.0f, (height() - m_image->height()) / 2.0f);
        QRect rect = QRect(m_top.x, m_top.y, m_image->width(), m_image->height());
        m_painter.drawImage(rect, *m_image);
    }
    m_painter.setPen(m_pen);
    // render Primitive
    for (auto& s : m_shapeList)
    {
        s->Draw(m_painter);
    }
    if (m_shape != nullptr)
    {
        m_shape->Draw(m_painter);
    }
    m_painter.end();
}

void ImageWidget::mousePressEvent(QMouseEvent* event)
{
    m_pressPoint.set(std::clamp(event->pos().x() - m_top.x, 0.0f, (float)m_width), std::clamp(event->pos().y() - m_top.y, 0.0f, (float)m_height)) ;
    std::cout << "x:" << m_pressPoint.x << ", y:" << m_pressPoint.y << std::endl;
    if (m_drawStatus && Qt::LeftButton == event->button())
    {
        switch (m_primitiveType)
        {
        case Shape::Primitive::Line:
            m_shape = std::make_unique<Line>();
            break;
        case Shape::Primitive::Rect:
            m_shape = std::make_unique<Rect>();
            break;
        default:
            break;
        }
        if (m_shape != nullptr)
        {
            m_drawStatus = true;
            auto position = event->pos();
            m_starts.emplace_back(m_pressPoint.x, m_pressPoint.y);
            m_shape->setStart(position);
            m_shape->setEnd(position);
        }
        update();
    }
}

void ImageWidget::mouseMoveEvent(QMouseEvent* event)
{
    m_pressPoint.set(std::clamp(event->pos().x() - m_top.x, 0.0f, (float)m_width), std::clamp(event->pos().y() - m_top.y, 0.0f, (float)m_height)) ;
    if (m_drawStatus && m_shape != nullptr)
    {
        auto position = event->pos();
        m_shape->setEnd(position);
    }
    update();
}

void ImageWidget::mouseReleaseEvent(QMouseEvent* event)
{
    m_pressPoint.set(std::clamp(event->pos().x() - m_top.x, 0.0f, (float)m_width), std::clamp(event->pos().y() - m_top.y, 0.0f, (float)m_height)) ;
    if (m_drawStatus && m_shape != nullptr)
    {
        m_ends.emplace_back(m_pressPoint.x, m_pressPoint.y);
        m_shapeList.emplace_back(std::move(m_shape));
        m_shape = nullptr;
    }
    update();
}

void ImageWidget::actionNew()
{
    std::cout << "actionNew" << std::endl;
}

void ImageWidget::actionOpen()
{
    std::cout << "actionOpen" << std::endl;
    auto path = QFileDialog::getOpenFileName(nullptr, QString(), QString(), tr("Images (*.png *.xpm *.jpg *.bmp)"));
    //    QString path = "../resources/test.jpg";
    //    QString path = "../resources/monaLisa.bmp";
    if (m_image == nullptr)
    {
        m_image = std::make_unique<QImage>();
    }
    if (m_originImage == nullptr)
    {
        m_originImage = std::make_unique<QImage>();
    }
    if (!path.isEmpty())
    {
        m_image->load(path);
        *(m_originImage) = *(m_image);
        m_matOriginImage = std::make_unique<cv::Mat>(std::move(qImage2Mat(*m_originImage)));
        m_width = m_image->width();
        m_height = m_image->height();
        m_drawStatus = true;
    }
    update();
}

void ImageWidget::actionSave()
{
    std::cout << "actionSave" << std::endl;
    auto filename = QFileDialog::getSaveFileName(nullptr, tr("Save As"), "untitled.png", tr("Images(*.bmp *.jpg)"));
    if (!m_image && m_image->isNull())
    {
        m_image->save(filename);
    }
}

void ImageWidget::actionClose()
{
    std::cout << "actionClose" << std::endl;
    destroy();
    update();
}

void ImageWidget::actionUndo()
{
    std::cout << "actionUndo" << std::endl;
    *m_image = *m_originImage;
    update();
}

void ImageWidget::actionInvert()
{
    std::cout << "actionInvert" << std::endl;
    if (m_image != nullptr && m_matOriginImage != nullptr)
    {
        *m_image = mat2Qimage(channelSwap(*m_matOriginImage));
        update();
    }
}

void ImageWidget::actionGray()
{
    std::cout << "actionGray" << std::endl;
    if (m_image != nullptr && m_matOriginImage != nullptr)
    {
        *m_image = mat2Qimage(grayTest(*m_matOriginImage));
        update();
    }
}

void ImageWidget::actionIDW()
{
    std::cout << "actionIDW" << std::endl;
    renderWarping<IdwWarping>();
}

void ImageWidget::actionRBF()
{
    std::cout << "actionRBF" << std::endl;
    renderWarping<RbfWarping>();
}

void ImageWidget::actionMirrorH()
{
    std::cout << "actionMirrorH" << std::endl;
    if (m_image != nullptr && m_matOriginImage != nullptr)
    {
        *m_image = mat2Qimage(mirrorTest(*m_matOriginImage, true, false));
        update();
    }
}

void ImageWidget::actionMirrorV()
{
    std::cout << "actionMirrorV" << std::endl;
    if (m_image != nullptr && m_matOriginImage != nullptr)
    {
        *m_image = mat2Qimage(mirrorTest(*m_matOriginImage, false, true));
        update();
    }
}

void ImageWidget::actionOrigin()
{
    std::cout << "actionOrigin" << std::endl;
    if (m_image != nullptr && m_originImage != nullptr)
    {
        *m_image = *m_originImage;
        update();
    }
}

void ImageWidget::actionColorTransform()
{
    std::cout << "actionColorTransform" << std::endl;
    if (m_image != nullptr && m_originImage != nullptr)
    {
        *m_image = mat2Qimage(colorTransferBetweenImages());
        update();
    }
}

void ImageWidget::actionLine()
{
    m_primitiveType = Shape::Primitive::Line;
    std::cout << "actionLine" << std::endl;
}

void ImageWidget::actionRect()
{
    m_primitiveType = Shape::Primitive::Rect;
    std::cout << "actionRect" << std::endl;
}

void ImageWidget::actionAbout()
{
    std::cout << "actionAbout" << std::endl;
}

void ImageWidget::actionHelp()
{
    std::cout << "actionHelp" << std::endl;
}

void ImageWidget::destroy()
{
    m_image = nullptr;
    m_originImage = nullptr;
    m_shapeList.clear();
    m_shape = nullptr;
    m_starts.clear();
    m_ends.clear();
    m_drawStatus = false;
    m_width = 0;
    m_height = 0;
}

template <class Warping>
void ImageWidget::renderWarping()
{
    if (m_image != nullptr && !m_starts.empty() && !m_ends.empty())
    {
        if (m_warping == nullptr)
        {
            m_warping = std::make_unique<Warping>(m_starts, m_ends);
            m_warping->resize(m_width, m_height);
        }
        m_shapeList.clear();
        m_warping->resetFilledStatus();
        m_warping->setPointP(m_starts);
        m_warping->setPointQ(m_ends);
        m_image->fill(Qt::white);
        m_warping->render(*m_image, *m_originImage);
        update();
    }
}
