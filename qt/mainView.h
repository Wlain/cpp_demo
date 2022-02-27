//
// Created by cwb on 2022/2/21.
//

#ifndef CPP_DEMO_MAINVIEW_H
#define CPP_DEMO_MAINVIEW_H
#include <QGraphicsView>
#include "mainScene.h"

class MainView : public QGraphicsView
{
public:
    using QGraphicsView::QGraphicsView;
    ~MainView() override;

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
};

#endif //CPP_DEMO_MAINVIEW_H
