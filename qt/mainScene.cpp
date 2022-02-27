//
// Created by cwb on 2022/2/21.
//
#include "mainScene.h"

#include "imageWarping/idwWarping.h"
#include "imageWarping/rbfWarping.h"

#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <opencv2/opencv.hpp>
#include <vec3.h>

extern cv::Mat qImage2Mat(const QImage& image);
extern QImage mat2Qimage(const cv::Mat& mat);
extern cv::Mat colorTransferBetweenImages();
extern cv::Mat channelSwap(const cv::Mat& img);
extern cv::Mat grayTest(const cv::Mat& img);

MainScene::MainScene()
{
    m_group = std::make_unique<QGraphicsItemGroup>(createItemGroup(*(new QList<QGraphicsItem*>)));
}

MainScene::~MainScene() = default;

void MainScene::press(QMouseEvent* event)
{
    if (m_editable)
    {
        m_starts.emplace_back(event->pos().x(), event->pos().y());
        m_lines.emplace_back(new QGraphicsLineItem(QLineF(event->pos(), event->pos()))); //;
        m_lines.back()->setPen(QPen(QColor("red"), 2));
        m_group->addToGroup(m_lines.back().get());
    }
}

void MainScene::move(QMouseEvent* event)
{
    if (m_editable && m_lines.back() != nullptr)
    {
        QLineF newLine(m_lines.back()->line().p1(), event->pos());
        m_lines.back()->setLine(newLine);
    }
}

void MainScene::release(QMouseEvent* event)
{
    if (m_editable && m_lines.back() != nullptr)
    {
        QLineF newLine(m_lines.back()->line().p1(), event->pos());
        m_lines.back()->setLine(newLine);
        m_ends.emplace_back(event->pos().x(), event->pos().y());
    }
}

void MainScene::open()
{
    m_path = QFileDialog::getOpenFileName(nullptr, QString(), QString(), tr("Images (*.png *.xpm *.jpg *.bmp)")).toStdString();
    //    m_path = "../resources/test.jpg";
    //    m_path = "../resources/monaLisa.bmp";
    if (!m_path.empty())
    {
        QImage image(m_path.data());
        //        m_originImage = image.scaledToHeight((int)height());
        m_originImage = image;
        m_image = m_originImage;
        m_width = m_image.width();
        m_height = m_image.height();
        m_filled.resize(m_width * m_height);
        QPixmap p = QPixmap::fromImage(m_image);
        if (m_pixmapItem == nullptr)
        {
            m_pixmapItem = std::make_unique<QGraphicsPixmapItem>();
        }
        m_pixmapItem->setPixmap(p);
        m_group->addToGroup(m_pixmapItem.get());
        m_editable = true;
    }
}

void MainScene::save() const
{
    auto filename = QFileDialog::getSaveFileName(nullptr, tr("Save As"), "untitled.png", tr("Images(*.bmp *.png *.jpg)"));
    if (!m_image.isNull())
    {
        m_image.save(filename);
    }
}

void MainScene::invert()
{
    flushImage(channelSwap);
}

void MainScene::gray()
{
    flushImage(grayTest);
}

void MainScene::origin()
{
    if (!m_originImage.isNull())
    {
        QPixmap p = QPixmap::fromImage(m_originImage);
        m_pixmapItem->setPixmap(p);
    }
}

void MainScene::mirror(bool h, bool v)
{
    if (m_editable && !m_image.isNull())
    {
        m_image = std::move(m_originImage.mirrored(h, v));
        QPixmap p = QPixmap::fromImage(m_image);
        m_pixmapItem->setPixmap(p);
    }
}

void MainScene::mirrorH()
{
    mirror(true, false);
}

void MainScene::mirrorV()
{
    mirror(false, true);
}

void MainScene::deleteAll()
{
    /// DOTO
}

void MainScene::calcIDW()
{
    if (m_editable && !m_image.isNull() && !m_starts.empty() && !m_ends.empty())
    {
        std::fill(m_filled.begin(), m_filled.end(), false);
        for (const auto& line : m_lines)
        {
            removeItem(line.get());
        }
        if (m_warping == nullptr)
        {
            m_warping = std::make_unique<IdwWarping>(m_starts, m_ends);
        }
        m_warping->setPointP(m_starts);
        m_warping->setPointQ(m_ends);
        m_image.fill(Qt::white);
        /// 对函数进行插值
        for (int i = 0; i < m_image.width(); ++i)
        {
            for (int j = 0; j < m_image.height(); ++j)
            {
                Vector2 point{ (float)i, (float)j };
                point = m_warping->targetFunction(point);
                if (point.x >= 0.0f && point.x < (float)m_image.width() && point.y >= 0.0f && point.y < (float)m_image.height())
                {
                    m_filled[getFrameBufferIndex((int)point.x, (int)point.y)] = true;
                    m_image.setPixel((int)point.x, (int)point.y, m_originImage.pixel(i, j));
                }
            }
        }
        for (int i = 1; i < m_image.width() - 1; ++i)
        {
            for (int j = 1; j < m_image.height() - 1; ++j)
            {
                if (!m_filled[getFrameBufferIndex(i, j)])
                {
                    Vec3i sum{ 0, 0, 0 };
                    auto index = 0;
                    // 如果像素周边有像素
                    if (m_filled[getFrameBufferIndex(i - 1, j - 1)] || m_filled[getFrameBufferIndex(i, j - 1)] || m_filled[getFrameBufferIndex(i + 1, j - 1)] ||
                        m_filled[getFrameBufferIndex(i - 1, j)] || m_filled[getFrameBufferIndex(i + 1, j)] ||
                        m_filled[getFrameBufferIndex(i - 1, j + 1)] || m_filled[getFrameBufferIndex(i, j + 1)] || m_filled[getFrameBufferIndex(i + 1, j + 1)])
                    {
                        for (int n = i - 1; n <= i + 1; ++n)
                        {
                            for (int m = j - 1; m <= j + 1; ++m)
                            {
                                if (n == i && m == j) break;
                                if (m_filled[getFrameBufferIndex(n, m)])
                                {
                                    auto color = m_image.pixelColor(n, m);
                                    sum += Vec3i(color.red(), color.green(), color.blue());
                                    index++;
                                }
                            }
                        }
                    }
                    if (!sum.isZero())
                    {
                        const auto& avg = sum / index;
                        m_filled[getFrameBufferIndex(i, j)] = true;
                        m_image.setPixel(i, j, qRgba(avg.x, avg.y, avg.z, 255));
                    }
                }
            }
        }
        QPixmap p = QPixmap::fromImage(m_image);
        m_pixmapItem->setPixmap(p);
    }
}

void MainScene::calcRBF()
{
    if (m_editable && !m_image.isNull() && !m_starts.empty() && !m_ends.empty())
    {
        if (m_warping == nullptr)
        {
            m_warping = std::make_unique<RbfWarping>(m_starts, m_ends);
        }
    }
}

void MainScene::flushImage(const std::function<cv::Mat(const cv::Mat&)>& func)
{
    if (m_editable && !m_image.isNull())
    {
        m_image = mat2Qimage(func(qImage2Mat(m_originImage)));
        QPixmap p = QPixmap::fromImage(m_image);
        m_pixmapItem->setPixmap(p);
    }
}

void MainScene::colorTransform()
{
    if (m_editable && !m_image.isNull())
    {
        m_image = mat2Qimage(colorTransferBetweenImages());
        QPixmap p = QPixmap::fromImage(m_image);
        m_pixmapItem->setPixmap(p);
    }
}
