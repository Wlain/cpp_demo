//
// Created by william on 2022/2/19.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainWindow.h"

#include "mainScene.h"
#include "mainView.h"

#include <QPixmap>
#include <iostream>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent)
{
    setupUi(this);
    setWindowTitle("imageWarping");
}

MainWindow::~MainWindow() = default;

void MainWindow::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), QPixmap("../resources/test.jpg"));
}

void MainWindow::on_actionnew_triggered()
{
    std::cout << "on_actionnew_triggered" << std::endl;
}

void MainWindow::on_actionopen_triggered()
{
    std::cout << "on_actionopen_triggered" << std::endl;
}

void MainWindow::on_actionsave_triggered()
{
    std::cout << "on_actionsave_triggered" << std::endl;
}

void MainWindow::on_actionclose_triggered()
{
    std::cout << "on_actionclose_triggered" << std::endl;
}

void MainWindow::on_actionundo_triggered()
{
    std::cout << "on_actionundo_triggered" << std::endl;
}

void MainWindow::on_actionIDW_triggered()
{
    std::cout << "on_actionnewIDW_triggered" << std::endl;
}

void MainWindow::on_actionRBF_triggered()
{
    std::cout << "on_actionnewRBF_triggered" << std::endl;
}
