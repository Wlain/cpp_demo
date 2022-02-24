//
// Created by cwb on 2022/2/21.
//
#include "mainScene.h"

#include "idwWarping.h"
#include "rbfWarping.h"
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
    if(!m_path.empty())
    {
        m_image = std::make_unique<QImage>(m_path.data());
        if (m_image != nullptr)
        {
            QPixmap p = QPixmap::fromImage(*m_image.get());
            m_pixmapItem = std::make_unique<QGraphicsPixmapItem>();
            m_pixmapItem->setPixmap(p);
            m_group->addToGroup(m_pixmapItem.get());
        }
    }
}

void MainScene::save() const
{

}

void MainScene::invert()
{
    if(m_image != nullptr && !m_image->isNull())
    {
        m_image->invertPixels();
        QPixmap p = QPixmap::fromImage(*m_image);
        m_pixmapItem->setPixmap(p);
    }
}

void MainScene::grayScale()
{
}

void MainScene::restore()
{
}

void MainScene::mirrorH()
{
}

void MainScene::mirrorV()
{
}

void MainScene::deleteAll()
{
}

void MainScene::process()
{
}

void MainScene::setAlgorithmType(MainScene::AlgorithmType type)
{
}
