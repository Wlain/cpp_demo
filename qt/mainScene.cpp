//
// Created by cwb on 2022/2/21.
//
#include "mainScene.h"

#include <QGraphicsPixmapItem>
#include <memory>
MainScene::MainScene()
{
    m_group = std::make_unique<QGraphicsItemGroup>();
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
}

void MainScene::save() const
{
}

void MainScene::invert()
{
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
