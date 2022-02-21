//
// Created by william on 2022/2/19.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainWindow.h"

#include "ui_MainWindow.h"

#include <QAction>
#include <QMessageBox>
#include <iostream>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createButtons();
    creatPushButtons();
    createRadioButton();
    creatCheckBox();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createButtons()
{
    m_actionTest = new QAction(tr("&Hello world test"), this);

    m_menuTest = menuBar()->addMenu(tr("&Main"));
    m_menuTest->addAction(m_actionTest);

    m_toolbarTest = addToolBar(tr("&Main"));
    m_toolbarTest->addAction(m_actionTest);

    connect(m_actionTest, &QAction::triggered, this, &MainWindow::test);
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

void MainWindow::test()
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
