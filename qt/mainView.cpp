//
// Created by cwb on 2022/2/21.
//

#include "mainView.h"
#include <iostream>
MainView::~MainView() = default;

void MainView::mousePressEvent(QMouseEvent* event)
{
    std::cout << "x:" << event->pos().x() << ", y:" << event->pos().y() << std::endl;
    m_warping->press(event);
}

void MainView::mouseMoveEvent(QMouseEvent* event)
{
    std::cout << "x:" << event->pos().x() << ", y:" << event->pos().y() << std::endl;
    m_warping->move(event);
}

void MainView::mouseReleaseEvent(QMouseEvent* event)
{
    std::cout << "x:" << event->pos().x() << ", y:" << event->pos().y() << std::endl;
    m_warping->release(event);
}

void MainView::setAlgorithmType(MainScene::AlgorithmType type)
{

}
