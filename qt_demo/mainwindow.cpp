//
// Created by cwb on 2022/2/9.
//

// You may need to build the project (run Qt uic code generator) to get "ui_mainwindow.h" resolved

#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <QAction>
#include <QMessageBox>
#include <QMenuBar>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    setStyleSheet("padding:10px 4px;");
    m_view = new ViewWidget();
    setCentralWidget(m_view);
    createAction();
    creatMenu();
}

MainWindow::~MainWindow()
{
    delete m_ui;
    delete m_view;
}

void MainWindow::helloWorld()
{
    //    QMessageBox::information(NULL, "Hello", "Hello, Qt", QMessageBox::Yes);
}

void MainWindow::creatMenu()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_openAction);
    m_fileMenu->addAction(m_aboutAction);

    m_figureMenu = menuBar()->addMenu(tr("&Figure"));
    m_figureMenu->addAction(m_lineAction);
    m_figureMenu->addAction(m_rectAction);
    m_figureMenu->addAction(m_ellipseAction);
    m_figureMenu->addAction(m_polyAction);
    m_figureMenu->addAction(m_freeHandAction);

    m_editMenu = menuBar()->addMenu(tr("&Edit"));
    m_editMenu->addAction(m_undoAction);
    m_editMenu->addAction(m_redoAction);
    m_editMenu->addAction(m_clearAction);
}

void MainWindow::createToolBar()
{
}

void MainWindow::createAction()
{
    m_aboutAction = new QAction(QIcon("../icon/about.png"), tr("&About"), this);
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::aboutBox);

        m_lineAction = new QAction(QIcon("../icon/line.png"),tr("&Line"), this);
        connect(m_lineAction, SIGNAL(triggered()), m_view, SLOT(setLine()));

        m_rectAction = new QAction(QIcon("../icon/rect.png"),tr("&Rect"), this);
        connect(m_rectAction, &QAction::triggered, m_view, &ViewWidget::setRect);

        m_ellipseAction = new QAction(QIcon("../icon/elli.png"),tr("&Ellipse"), this);
        connect(m_ellipseAction, &QAction::triggered, m_view, &ViewWidget::setEllipse);

        m_polyAction = new QAction(QIcon("../icon/poly.png"),tr("&Polygon"), this);
        connect(m_polyAction, &QAction::triggered, m_view, &ViewWidget::setPoly);

        m_freeHandAction = new QAction(QIcon("../icon/hand.png"),tr("&FreeHand"), this);
        connect(m_freeHandAction, &QAction::triggered, m_view, &ViewWidget::setFreeHand);

//        m_colorAction = new QAction(QIcon("../icon/color.png"),tr("&Color"), this);
//        connect(m_colorAction, &QAction::triggered,this,&MainWindow::color_slot);
//        connect(this,&MainWindow::change_color,m_view,&ViewWidget::setColor);
//
//        m_widthAction = new QAction(QIcon("../icon/width.png"),tr("&Width"),this);
//        connect(m_widthAction, &QAction::triggered,this,&MiniDraw::width_slot);
//        connect(this,&MainWindow::change_width,m_view,&ViewWidget::setWidth);

        m_openAction = new QAction(QIcon("../icon/open.png"),tr("&Insert Picture"),this);
        connect(m_openAction, &QAction::triggered, m_view, &ViewWidget::setBackground);

        m_undoAction = new QAction(QIcon("../icon/undo.png"),tr("&Undo"),this);
        connect(m_undoAction ,&QAction::triggered, m_view, &ViewWidget::undo);

        m_redoAction = new QAction(QIcon("../icon/redo.png"),tr("&Redo"),this);
        connect(m_redoAction, &QAction::triggered, m_view, &ViewWidget::redo);

        m_clearAction= new QAction(QIcon("../icon/clear.png"),tr("&Clear"),this);
        connect(m_clearAction, &QAction::triggered, m_view, &ViewWidget::clear);
}

void MainWindow::aboutBox()
{
    QMessageBox::about(this, tr("About"), tr("MiniDraw"));
}