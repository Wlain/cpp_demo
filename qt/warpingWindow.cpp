//
// Created by william on 2022/2/19.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "warpingWindow.h"

#include "mainScene.h"
#include "mainView.h"

#include <QPixmap>
#include <iostream>

WarpingWindow::WarpingWindow(QWidget* parent) :
    QMainWindow(parent)
{
    setupUi(this);
    setWindowTitle("imageWarping");
    m_mainView = std::make_unique<MainView>();
    m_scene = std::make_unique<MainScene>();
    m_mainView->setScene(m_scene.get());
    setCentralWidget(m_mainView.get());
    m_scene->setSceneRect(0,0,width(),height());
    m_mainView->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    m_mainView->setRenderHint(QPainter::Antialiasing);
}

WarpingWindow::~WarpingWindow() = default;

//void WarpingWindow::paintEvent(QPaintEvent* event)
//{
//    QPainter painter(this);
//    painter.drawPixmap(0, 0, width(), height(), QPixmap("../resources/test.jpg"));
//}

void WarpingWindow::on_actionnew_triggered()
{
    m_scene->open();
    m_scene->update(m_mainView->rect());
    std::cout << "on_actionnew_triggered" << std::endl;
}

void WarpingWindow::on_actionopen_triggered()
{
    m_scene->open();
    m_scene->update(m_mainView->rect());
    std::cout << "on_actionopen_triggered" << std::endl;
}

void WarpingWindow::on_actionsave_triggered()
{
    m_scene->save();
    std::cout << "on_actionsave_triggered" << std::endl;
}

void WarpingWindow::on_actionclose_triggered()
{
    std::cout << "on_actionclose_triggered" << std::endl;
}

void WarpingWindow::on_actionundo_triggered()
{
    m_scene->restore();
    std::cout << "on_actionundo_triggered" << std::endl;
}

void WarpingWindow::on_actionIDW_triggered()
{
    m_scene->setAlgorithmType(MainScene::AlgorithmType::IDW);
    m_scene->process();
    std::cout << "on_actionnewIDW_triggered" << std::endl;
}

void WarpingWindow::on_actionRBF_triggered()
{
    m_scene->setAlgorithmType(MainScene::AlgorithmType::RBF);
    m_scene->process();
    std::cout << "on_actionnewRBF_triggered" << std::endl;
}

void WarpingWindow::on_actionrevert_triggered()
{
    m_scene->invert();
    std::cout << "on_actionrevert_triggered" << std::endl;
}

void WarpingWindow::on_actiongray_triggered()
{
    m_scene->grayScale();
    std::cout << "on_actiongray_triggered" << std::endl;
}
