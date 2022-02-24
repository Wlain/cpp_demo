//
// Created by cwb on 2022/2/21.
//
#include "mainScene.h"

#include "idwWarping.h"
#include "rbfWarping.h"
#include "vector3.h"

#include <QFileDialog>
#include <QGraphicsPixmapItem>

MainScene::MainScene()
{
    m_group = std::make_unique<QGraphicsItemGroup>(createItemGroup(*(new QList<QGraphicsItem*>)));
    m_pixmapItem = std::make_unique<QGraphicsPixmapItem>();
}

MainScene::~MainScene() = default;

void MainScene::press(QMouseEvent* event)
{
}

void MainScene::move(QMouseEvent* event)
{
}

void MainScene::release(QMouseEvent* event)
{
}

void MainScene::open()
{
    m_path = QFileDialog::getOpenFileName(0, QString(), QString(), tr("Images (*.png *.xpm *.jpg *.bmp)")).toStdString();
    //    m_path = "../resources/test.jpg";
    if (!m_path.empty())
    {
        QImage image(m_path.data());
        m_OriginImage = image.scaledToHeight(height());
        m_image = m_OriginImage;
        QPixmap p = QPixmap::fromImage(m_image);
        m_pixmapItem = std::make_unique<QGraphicsPixmapItem>();
        m_pixmapItem->setPixmap(p);
        m_group->addToGroup(m_pixmapItem.get());
    }
}

void MainScene::save() const
{
}

void MainScene::invert()
{
    if (!m_image.isNull())
    {
        m_image.invertPixels();
        QPixmap p = QPixmap::fromImage(m_image);
        m_pixmapItem->setPixmap(p);
    }
}

void MainScene::gray()
{
    if (!m_image.isNull())
    {
        for (int i = 0; i < m_image.width(); ++i)
        {
            for (int j = 0; j < m_image.height(); ++j)
            {
                auto pixel = m_image.pixel(i, j);
                Vector3 weight{ 0.2125, 0.7154, 0.0721 };
                Vector3 color{ (float)qRed(pixel), (float)qGreen(pixel), (float)qBlue(pixel) };
                auto gray = weight.dot(color);
                m_image.setPixel(i, j, qRgb((int)gray, (int)gray, (int)gray));
            }
        }
        QPixmap p = QPixmap::fromImage(m_image);
        m_pixmapItem->setPixmap(p);
    }
}

void MainScene::origin()
{
    if (!m_OriginImage.isNull())
    {
        QPixmap p = QPixmap::fromImage(m_OriginImage);
        m_pixmapItem->setPixmap(p);
    }
}

void MainScene::mirror(bool h, bool v)
{
    if (!m_image.isNull())
    {
        m_image = std::move(m_image.mirrored(h, v));
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

void MainScene::setAlgorithmType(MainScene::AlgorithmType type)
{
}
