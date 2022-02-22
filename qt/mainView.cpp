//
// Created by cwb on 2022/2/21.
//

#include "mainView.h"

#include "idwWarping.h"
#include "rbfWarping.h"
#include <iostream>

MainView::MainView(AlgorithmType type)
{
//    if (type == AlgorithmType::IDW)
//    {
//        m_warping = std::make_unique<IdwWarping>();
//    }
//    else if (type == AlgorithmType::RBF)
//    {
//        m_warping = std::make_unique<RbfWarping>();
//    }
}

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
