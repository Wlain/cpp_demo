//
// Created by cwb on 2022/2/9.
//

// You may need to build the project (run Qt uic code generator) to get "ui_mainwindow.h" resolved

#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <qmessagebox.h>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    createButtons();
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::helloWorld()
{
    QMessageBox::about(this, tr("Hello world"),
                       tr("Well done! Go ahead."));
}

void MainWindow::createButtons()
{
    m_action = new QAction(tr("&Hello world"), this);
    connect(m_action, &QAction::triggered, this, &MainWindow::helloWorld);
    m_meum = menuBar()->addMenu(tr("&Main"));
    m_meum->addAction(m_action);
    m_toolbar = addToolBar(tr("&Main"));
    m_toolbar->addAction(m_action);
}