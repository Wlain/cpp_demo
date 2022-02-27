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
    setWindowTitle("ImageProcess");
    setupUi(this);
    connect(m_actionNew, &QAction::triggered, this, &WarpingWindow::actionNew);
    connect(m_actionOpen, &QAction::triggered, this, &WarpingWindow::actionOpen);
    connect(m_actionSave, &QAction::triggered, this, &WarpingWindow::actionSave);
    connect(m_actionClose, &QAction::triggered, this, &WarpingWindow::actionClose);
    connect(m_actionIDW, &QAction::triggered, this, &WarpingWindow::actionIDW);
    connect(m_actionRBF, &QAction::triggered, this, &WarpingWindow::actionRBF);
    connect(m_actionUndo, &QAction::triggered, this, &WarpingWindow::actionUndo);
    connect(m_actionRevert, &QAction::triggered, this, &WarpingWindow::actionRevert);
    connect(m_actionGray, &QAction::triggered, this, &WarpingWindow::actionGray);
    connect(m_actionMirrorH, &QAction::triggered, this, &WarpingWindow::actionMirrorH);
    connect(m_actionMirrorV, &QAction::triggered, this, &WarpingWindow::actionMirrorV);
    connect(m_actionOrigin, &QAction::triggered, this, &WarpingWindow::actionOrigin);
    connect(m_actionColorTransform, &QAction::triggered, this, &WarpingWindow::actionColorTransform);

    m_mainView = std::make_unique<MainView>();
    m_scene = std::make_unique<MainScene>();
    m_mainView->setScene(m_scene.get());
    setCentralWidget(m_mainView.get());
    m_scene->setSceneRect(0, 0, width(), height());
    m_mainView->setAlignment(Qt::AlignCenter);
    m_mainView->setRenderHint(QPainter::Antialiasing);
}

WarpingWindow::~WarpingWindow() = default;

void WarpingWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Space)
    {
        m_scene->open();
        m_scene->update(m_mainView->rect());
    }
}

void WarpingWindow::actionNew()
{
    m_scene->open();
    m_scene->update(m_mainView->rect());
    std::cout << "actionNew" << std::endl;
}

void WarpingWindow::actionOpen()
{
    m_scene->open();
    m_scene->update(m_mainView->rect());
    std::cout << "actionOpen" << std::endl;
}

void WarpingWindow::actionSave()
{
    m_scene->save();
    std::cout << "actionSave" << std::endl;
}

void WarpingWindow::actionClose()
{
    m_scene->deleteAll();
    std::cout << "actionClose" << std::endl;
}

void WarpingWindow::actionUndo()
{
    m_scene->origin();
    std::cout << "actionUndo" << std::endl;
}

void WarpingWindow::actionRevert()
{
    m_scene->invert();
    std::cout << "actionRevert" << std::endl;
}

void WarpingWindow::actionGray()
{
    m_scene->gray();
    std::cout << "actionGray" << std::endl;
}

void WarpingWindow::actionIDW()
{
    m_scene->calcIDW();
    std::cout << "actionIDW" << std::endl;
}

void WarpingWindow::actionRBF()
{
    m_scene->calcRBF();
    std::cout << "actionRBF" << std::endl;
}

void WarpingWindow::actionMirrorH()
{
    m_scene->mirrorH();
    std::cout << "actionMirrorH" << std::endl;
}

void WarpingWindow::actionMirrorV()
{
    m_scene->mirrorV();
    std::cout << "actionMirrorV" << std::endl;
}

void WarpingWindow::actionOrigin()
{
    m_scene->origin();
    std::cout << "actionOrigin" << std::endl;
}

void WarpingWindow::actionColorTransform()
{
    m_scene->colorTransform();
    std::cout << "actionOrigin" << std::endl;
}