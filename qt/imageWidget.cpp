//
// Created by william on 2022/3/2.
//

#include "imageWidget.h"

#include "imageWarping/idwWarping.h"
#include "imageWarping/rbfWarping.h"
#include "primitive/line.h"
#include "primitive/oval.h"
#include "primitive/pen.h"
#include "primitive/rect.h"

#include <QColorDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QKeyEvent>
#include <QPainter>
#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>
#include <queue>

extern cv::Mat qImage2Mat(const QImage& image);
extern QImage mat2Qimage(const cv::Mat& mat);
extern cv::Mat colorTransferBetweenImages(const cv::Mat& inputSrc, const cv::Mat& inputDst);
extern cv::Mat channelSwap(const cv::Mat& img);
extern cv::Mat grayTest(const cv::Mat& img);
extern cv::Mat mirrorTest(const cv::Mat& img, bool horizontal, bool vertical);
extern cv::Mat alphaBlend(const cv::Mat& src, const cv::Mat& dst, const cv::Mat& mask);
extern bool imageValid(std::unique_ptr<QImage>& image);
extern void cropImage(cv::Mat& image);
extern cv::Mat ddaLine(const Vector2& begin, const Vector2& end, cv::Mat& img);
extern void ddaLine(const Vector2& begin, const Vector2& end, QImage& image);

#define LATTICE 10      // 像素网格的单位宽度
#define LEFT_MARGIN 10  // 左侧边界距离
#define UPPER_MARGIN 10 // 上方边界距离

ImageWidget::ImageWidget()
{
    m_image = std::make_unique<QImage>();
    m_originImage = std::make_unique<QImage>();
    m_textImage = std::make_unique<QImage>(100, 100, QImage::Format_RGB32);
    m_pen = QPen(QColor("red"), 2);
    m_drawStatus = true;
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
    m_painter.setRenderHints(QPainter::Antialiasing, true);
    // clear background
    {
        m_painter.setBrush(Qt::white);
        m_painter.drawRect(0, 0, width(), height());
    }

    // 绘制网格
    {
        m_painter.setPen(Qt::gray);
        m_painter.setBrush(QColor(240, 240, 240, 255));
        for (int x = LEFT_MARGIN; x < width() - LATTICE; x += LATTICE)
        {
            for (int y = UPPER_MARGIN; y < height() - LATTICE; y += LATTICE)
            {
                m_painter.drawRect(x, y, LATTICE, LATTICE);
            }
        }
    }

    // render image
    if (m_image != nullptr)
    {
        m_top.set((float)(width() - m_image->width()) / 2.0f, (float)(height() - m_image->height()) / 2.0f);
        QRect rect = QRect((int)m_top.x, (int)m_top.y, m_image->width(), m_image->height());
        m_painter.drawImage(rect, *m_image);
    }
    // render Primitive
    for (auto& s : m_shapeList)
    {
        s->draw(m_primitivePainter, this);
    }
    if (m_shape != nullptr)
    {
        m_shape->draw(m_primitivePainter, this);
    }
    if (m_sourceImage.isReady)
    {
        m_painter.drawImage(106, 50, *m_sourceImage.qImage);
    }
    if (m_targetImage.isReady)
    {
        m_painter.drawImage(618, 50, *m_targetImage.qImage);
    }
    if (m_maskImage.qImage != nullptr)
    {
        m_painter.drawImage(106, m_maskImage.qImage->height() + 100, *m_maskImage.qImage);
    }
    if (m_resultImage.isReady)
    {
        m_painter.drawImage(618, m_resultImage.qImage->height() + 100, *m_resultImage.qImage);
    }

    m_painter.end();
}

void ImageWidget::calcPressPoint(QMouseEvent* event)
{
    if (imageValid(m_image))
    {
        m_pressPoint.set(std::clamp((float)event->pos().x() - m_top.x, 0.0f, (float)m_width), std::clamp((float)event->pos().y() - m_top.y, 0.0f, (float)m_height));
    }
    else
    {
        m_pressPoint.set((float)event->pos().x(), (float)event->pos().y());
    }
}

void ImageWidget::mousePressEvent(QMouseEvent* event)
{
    calcPressPoint(event);
//    std::cout << "x:" << m_pressPoint.x << ", y:" << m_pressPoint.y << std::endl;
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
        case Shape::Primitive::Oval:
            m_shape = std::make_unique<Oval>();
            break;
        case Shape::Primitive::Pen:
            m_shape = std::make_unique<Pen>();
            break;
        default:
            return;
        }
        if (m_shape != nullptr)
        {
            m_drawStatus = true;
            auto position = event->pos();
            m_starts.emplace_back(m_pressPoint.x, m_pressPoint.y);
            m_shape->setStart(position);
            m_shape->setEnd(position);
            m_shape->setPen(m_pen);
        }
        update();
    }
}

void ImageWidget::mouseMoveEvent(QMouseEvent* event)
{
    calcPressPoint(event);
//    std::cout << "x:" << m_pressPoint.x << ", y:" << m_pressPoint.y << std::endl;
    if (m_drawStatus && m_shape != nullptr)
    {
        auto position = event->pos();
        m_shape->setEnd(position);
    }
    /// 绘制mask缘图
    auto& sourceQImage = m_sourceImage.qImage;
    if (sourceQImage != nullptr)
    {
        auto w = sourceQImage->width();
        auto h = sourceQImage->height();
        if (m_maskImage.qImage == nullptr)
        {
            m_maskImage.qImage = std::make_unique<QImage>(w, h, QImage::Format_RGB888);
            m_maskImage.qImage->fill(QColor(255, 255, 255));
        }

        bool buttonAtSource = event->x() >= 106 && event->x() < (sourceQImage->width() + 106) &&
            event->y() >= 50 && event->y() < (sourceQImage->height() + 50);
        if (buttonAtSource && (event->buttons() & Qt::LeftButton))
        {
            auto pos = m_lastEditPos;
            m_lastEditPos.set(event->x() - 106, event->y() - 50);
            if (!m_isFirstDrawEditPos)
            {
                ddaLine(m_lastEditPos, pos, *m_maskImage.qImage);
            }
            m_isFirstDrawEditPos = false;
        }
    }
    update();
}

void ImageWidget::mouseReleaseEvent(QMouseEvent* event)
{
    calcPressPoint(event);
//    std::cout << "x:" << m_pressPoint.x << ", y:" << m_pressPoint.y << std::endl;
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
    m_iconText = QInputDialog::getText(this, tr("william"), tr("请输入要保存的文字:"), QLineEdit::Normal).toStdString();
}

void ImageWidget::actionOpen()
{
    std::cout << "actionOpen" << std::endl;
    auto path = QFileDialog::getOpenFileName(nullptr, QString(), QString(), tr("Images (*.png *.xpm *.jpg *.bmp)"));
    //        QString path = GET_CURRENT(/resources/test.jpg);
    //        QString path = GET_CURRENT(/resources/monaLisa.bmp);
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
    auto filename = QFileDialog::getSaveFileName(nullptr, tr("Save As"), "icon", tr("Images(*.png *.jpg)"));
    if (imageValid(m_image))
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
    if (imageValid(m_image) && m_matOriginImage != nullptr)
    {
        *m_image = mat2Qimage(channelSwap(*m_matOriginImage));
        update();
    }
}

void ImageWidget::actionGray()
{
    std::cout << "actionGray" << std::endl;
    if (imageValid(m_image) && m_matOriginImage != nullptr)
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
    if (imageValid(m_image) && m_matOriginImage != nullptr)
    {
        *m_image = mat2Qimage(mirrorTest(*m_matOriginImage, true, false));
        update();
    }
}

void ImageWidget::actionMirrorV()
{
    std::cout << "actionMirrorV" << std::endl;
    if (imageValid(m_image) && m_matOriginImage != nullptr)
    {
        *m_image = mat2Qimage(mirrorTest(*m_matOriginImage, false, true));
        update();
    }
}

void ImageWidget::actionOrigin()
{
    std::cout << "actionOrigin" << std::endl;
    if (imageValid(m_image) && m_originImage != nullptr)
    {
        *m_image = *m_originImage;
        update();
    }
}

void ImageWidget::actionColorTransform()
{
    std::cout << "actionColorTransform" << std::endl;
    auto& sourceCvImage = m_sourceImage.matImage;
    auto& targetCvImage = m_targetImage.matImage;
    auto& maskCvImage = m_maskImage.matImage;
    if (sourceCvImage != nullptr && targetCvImage != nullptr && maskCvImage != nullptr)
    {
        if (m_resultImage.qImage == nullptr)
        {
            m_resultImage.qImage = std::make_unique<QImage>();
        }
        *m_resultImage.qImage = mat2Qimage(colorTransferBetweenImages(*targetCvImage, *sourceCvImage)).scaled(s_imageWidth, s_imageWidth * targetCvImage->rows / targetCvImage->cols);
        m_resultImage.isReady = true;
        update();
    }
}

void ImageWidget::actionLine()
{
    std::cout << "actionLine" << std::endl;
    m_primitiveType = Shape::Primitive::Line;
}

void ImageWidget::actionRect()
{
    std::cout << "actionRect" << std::endl;
    m_primitiveType = Shape::Primitive::Rect;
}

void ImageWidget::actionPen()
{
    std::cout << "actionPen" << std::endl;
    m_primitiveType = Shape::Primitive::Pen;
}

void ImageWidget::actionOval()
{
    std::cout << "actionOval" << std::endl;
    m_primitiveType = Shape::Primitive::Oval;
}

void ImageWidget::actionSetWidth()
{
    std::cout << "actionSetWidth" << std::endl;
    auto width = QInputDialog::getInt(this, QString("Set Width"), QString("Input width"), true, 1, 20, 1);
    m_pen.setWidth(width);
}

void ImageWidget::actionPalette()
{
    std::cout << "actionPalette" << std::endl;
    auto color = QColorDialog::getColor(Qt::white);
    m_pen.setColor(color);
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
    m_sourceImage.qImage = nullptr;
    m_sourceImage.matImage = nullptr;
    m_targetImage.qImage = nullptr;
    m_targetImage.matImage = nullptr;
    m_resultImage.qImage = nullptr;
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
        if (m_warping == nullptr || !std::is_same<std::remove_reference<decltype(*m_warping)>::type, Warping>())
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

void ImageWidget::actionOpenSourceImage()
{
    openImage(m_sourceImage);
}

void ImageWidget::actionOpenTargetImage()
{
    openImage(m_targetImage);
}

void ImageWidget::actionOpenMask()
{
    std::cout << "actionOpenMask" << std::endl;
    if (imageValid(m_maskImage.qImage))
    {
        /// BFS实现区域填充算法
        std::queue<Vec2i> pointQueue;
        pointQueue.push({ 0, 0 });
        auto blackColor = qRgb(0, 0, 0);
        int w = m_maskImage.qImage->width();
        int h = m_maskImage.qImage->height();
        while (!pointQueue.empty())
        {
            auto p = pointQueue.front();
            pointQueue.pop();
            for (int i = 0; i < m_offsets.size(); ++i)
            {
                int curX = p.x + m_offsets[i][0];
                int curY = p.y + m_offsets[i][1];
                if (curX >= 0 && curX < w &&
                    curY >= 0 && curY < h &&
                    m_maskImage.qImage->pixel(curX, curY) != blackColor)
                {
                    m_maskImage.qImage->setPixel(curX, curY, blackColor);
                    pointQueue.push({ curX, curY });
                }
            }
        }
        m_maskImage.matImage = std::make_unique<cv::Mat>(qImage2Mat(*m_maskImage.qImage));
    }
    else
    {
        openImage(m_maskImage);
    }
    update();
}

void ImageWidget::actionAlphaBlend()
{
    std::cout << "actionAlphaBlend" << std::endl;
    auto& sourceCvImage = m_sourceImage.matImage;
    auto& targetCvImage = m_targetImage.matImage;
    auto& maskCvImage = m_maskImage.matImage;
    if (sourceCvImage != nullptr && targetCvImage != nullptr && maskCvImage != nullptr)
    {
        if (m_resultImage.qImage == nullptr)
        {
            m_resultImage.qImage = std::make_unique<QImage>();
        }
        *m_resultImage.qImage = mat2Qimage(alphaBlend(*sourceCvImage, *targetCvImage, *maskCvImage)).scaled(s_imageWidth, s_imageWidth * targetCvImage->rows / targetCvImage->cols);
        m_resultImage.isReady = true;
        update();
    }
}

void ImageWidget::actionPossionImageBlend()
{
    std::cout << "actionPossionImageBlend" << std::endl;
}

void ImageWidget::openImage(ImageWrapper& image)
{
    std::string path = QFileDialog::getOpenFileName(nullptr, QString(), QString(), tr("Images (*.png *.xpm *.jpg *.bmp)")).toStdString();
    if (path.empty()) return;
    image.matImage = std::make_unique<cv::Mat>(cv::imread(path));
    auto width = image.matImage->cols;
    auto height = image.matImage->rows;
    cropImage(*image.matImage);
    cv::resize(*image.matImage, *image.matImage, cvSize(s_imageWidth, s_imageWidth));
    if (image.qImage == nullptr)
    {
        image.qImage = std::make_unique<QImage>();
    }
    auto ratio = height > width ? height / width : width / height;
    *image.qImage = mat2Qimage(*image.matImage).scaled(s_imageWidth, s_imageWidth * ratio);
    image.isReady = true;
    update();
}
