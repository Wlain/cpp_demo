//
// Created by cwb on 2022/2/21.
//

#include "mainView.h"
#include "mainScene.h"
#include <iostream>

MainView::~MainView() = default;

void MainView::mousePressEvent(QMouseEvent* event)
{
//    std::cout << "x:" << event->pos().x() << ", y:" << event->pos().y() << std::endl;
    auto* s = dynamic_cast<MainScene*>(scene());
    s->press(event);
}

void MainView::mouseMoveEvent(QMouseEvent* event)
{
//    std::cout << "x:" << event->pos().x() << ", y:" << event->pos().y() << std::endl;
    auto* s = dynamic_cast<MainScene*>(scene());
    s->move(event);
}

void MainView::mouseReleaseEvent(QMouseEvent* event)
{
//    std::cout << "x:" << event->pos().x() << ", y:" << event->pos().y() << std::endl;
    auto* s = dynamic_cast<MainScene*>(scene());
    s->release(event);
}

