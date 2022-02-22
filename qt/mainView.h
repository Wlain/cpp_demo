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
    enum class AlgorithmType
    {
        IDW,
        RBF
    };

public:
    explicit MainView(AlgorithmType type);
    ~MainView() override;

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    std::unique_ptr<MainScene> m_warping;
};

#endif //CPP_DEMO_MAINVIEW_H
