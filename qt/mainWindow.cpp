//
// Created by william on 2022/2/19.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainWindow.h"

#include "mainScene.h"
#include "mainView.h"

#include <QAction>
#include <QMessageBox>
#include <QPixmap>
#include <QTextStream>
#include <QToolButton>
#include <iostream>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent)
{
    setupUi(this);
    setWindowTitle("imageWarping");
    //    creatPushButtons();
    //    createButtons();
    //    creatCheckBox();
    //    createRadioButton();
}

MainWindow::~MainWindow() = default;

void MainWindow::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), QPixmap("../resources/test.jpg"));
}

/// == == == == == == = test code == == == == == == == == ///
void MainWindow::createButtons()
{
    m_actionTest = new QAction(tr("&Hello world buttonsTest"), this);
    //    m_menuTest = menuBar()->addMenu(tr("&Main"));
    //    m_menuTest->addAction(m_actionTest);
    m_toolbarTest = addToolBar(tr("&Main"));
    m_toolbarTest->addAction(m_actionTest);
    connect(m_actionTest, &QAction::triggered, this, &MainWindow::buttonsTest);
}

void MainWindow::creatPushButtons()
{
    m_pushButtonTest = new QPushButton("Hello", this);
    m_pushButtonTest->move(0, 20);
    m_pushButtonTest->setStyleSheet("QPushButton{font:10px;}");
    connect(m_pushButtonTest, &QPushButton::clicked, this, &MainWindow::clickedTest);
}

void MainWindow::createRadioButton()
{
    m_radioButtonTest = new QRadioButton("RadioButton", this);
    m_radioButtonTest->move(0, 40);
    connect(m_radioButtonTest, &QRadioButton::clicked, this, &MainWindow::radioButtonTest);
}

void MainWindow::creatCheckBox()
{
    m_checkBoxTest = new QCheckBox("RadioButton", this);
    m_checkBoxTest->move(0, 60);
    connect(m_checkBoxTest, &QCheckBox::clicked, this, &MainWindow::checkBoxTest);
}

void MainWindow::buttonsTest()
{
    std::cout << "helloWorld" << std::endl;
    QMessageBox::about(this, tr("Hello world"), tr("Well done! Go ahead."));
}

void MainWindow::clickedTest()
{
    std::cout << "clickedTest" << std::endl;
    QMessageBox::about(this, tr("clickedTest"), tr("Well done! Go ahead."));
}

void MainWindow::radioButtonTest()
{
    std::cout << "radioButtonTest" << std::endl;
    QMessageBox::about(this, tr("radioButtonTest"), tr("Well done! Go ahead."));
}

void MainWindow::checkBoxTest()
{
    std::cout << "checkBoxTest" << std::endl;
    QMessageBox::about(this, tr("checkBoxTest"), tr("Well done! Go ahead."));
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
