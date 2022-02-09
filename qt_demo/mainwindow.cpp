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
    m_action = new QAction(tr("&hello"),this);

    m_meum = menuBar()->addMenu(tr("&Test"));
    m_meum->addAction(m_action);

    m_toolbar = addToolBar(tr("&Main"));
    m_toolbar->addAction(m_action);

    connect(m_action, &QAction::triggered, this, &MainWindow::helloWorld);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::helloWorld()
{
    QMessageBox::information(NULL, "Hello", "Hello, Qt", QMessageBox::Yes);
}
