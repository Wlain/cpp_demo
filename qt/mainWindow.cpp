//
// Created by william on 2022/2/19.
//

#include "mainWindow.h"

#include "imageWidget.h"

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent, {}) // QMainWindow's default flag is WType_TopLevel
{
    auto* fileMenu = new QMenu(this);
    auto* editMenu = new QMenu(this);
    auto* helpMenu = new QMenu(this);
    QToolBar* toolBar = addToolBar(tr("File"));
    fileMenu->setStatusTip(tr("File menu"));
    m_imageWidget = std::make_unique<ImageWidget>();
    m_imageWidget->setFocusPolicy(Qt::ClickFocus);

    setGeometry(300, 150, 800, 720);
    setCentralWidget(m_imageWidget.get());
    setWindowTitle("MainWindow");

    auto* action = new QAction(QIcon("../resources/icons/filenew.png"), tr("&New"), this);
    action->setToolTip(tr("New"));
    action->setShortcut(QKeySequence::New);
    action->setStatusTip(tr("Create a new fileMenu"));
    connect(action, &QAction::triggered, m_imageWidget.get(), &ImageWidget::actionNew);
    fileMenu->addAction(action);
    toolBar->addAction(action);

    action = new QAction(QIcon("../resources/icons/fileopen.png"), tr("&Open"), this);
    action->setToolTip(tr("Open"));
    action->setShortcut(QKeySequence::Open);
    action->setStatusTip(tr("Open an existing image"));
    connect(action, &QAction::triggered, m_imageWidget.get(), &ImageWidget::actionOpen);
    fileMenu->addAction(action);
    toolBar->addAction(action);

    action = new QAction(QIcon("../resources/icons/filesave.png"), tr("&Save"), this);
    action->setToolTip(tr("Save"));
    action->setShortcut(QKeySequence::Save);
    action->setStatusTip(tr("Save the image to disk"));
    connect(action, &QAction::triggered, m_imageWidget.get(), &ImageWidget::actionSave);
    fileMenu->addAction(action);
    toolBar->addAction(action);

    action = new QAction(QIcon("../resources/icons/remove.png"), tr("&Closed"), this);
    action->setToolTip(tr("Open"));
    action->setShortcut(QKeySequence::Close);
    action->setStatusTip(tr("Closed all"));
    connect(action, &QAction::triggered, m_imageWidget.get(), &ImageWidget::actionClose);
    fileMenu->addAction(action);
    toolBar->addAction(action);

    action = new QAction(tr("&Save &As..."), this);
    action->setToolTip(tr("Save &As..."));
    action->setShortcut(QKeySequence::SaveAs);
    action->setStatusTip(tr("Save the image under a new name"));
    connect(action, &QAction::triggered, m_imageWidget.get(), &ImageWidget::actionSave);
    fileMenu->addAction(action);

    action = new QAction(QIcon("../resources/icons/undo.png"), tr("&Undo"), this);
    action->setToolTip(tr("Undo"));
    action->setShortcut(QKeySequence::Undo);
    action->setStatusTip(tr("Undo"));
    connect(action, &QAction::triggered, m_imageWidget.get(), &ImageWidget::actionUndo);
    fileMenu->addAction(action);
    toolBar->addAction(action);

    action = new QAction(tr("&ColorTransform"), this);
    action->setToolTip(tr("ColorTransform"));
    action->setStatusTip(tr("ColorTransform"));
    connect(action, &QAction::triggered, m_imageWidget.get(), &ImageWidget::actionColorTransform);
    editMenu->addAction(action);
    toolBar->addAction(action);

    action = new QAction(tr("&Gray"), this);
    action->setToolTip(tr("gray"));
    action->setStatusTip(tr("gray"));
    connect(action, &QAction::triggered, m_imageWidget.get(), &ImageWidget::actionGray);
    editMenu->addAction(action);
    toolBar->addAction(action);

    action = new QAction(tr("&MirrorH"), this);
    action->setToolTip(tr("MirrorH"));
    action->setStatusTip(tr("MirrorH"));
    connect(action, &QAction::triggered, m_imageWidget.get(), &ImageWidget::actionMirrorH);
    editMenu->addAction(action);
    toolBar->addAction(action);

    action = new QAction(tr("&MirrorV"), this);
    action->setToolTip(tr("MirrorV"));
    action->setStatusTip(tr("MirrorV"));
    connect(action, &QAction::triggered, m_imageWidget.get(), &ImageWidget::actionMirrorV);
    editMenu->addAction(action);
    toolBar->addAction(action);

    action = new QAction(tr("&Inverse"), this);
    action->setToolTip(tr("Inverse"));
    action->setStatusTip(tr("Invert all pixel value in the image"));
    connect(action, &QAction::triggered, m_imageWidget.get(), &ImageWidget::actionInvert);
    editMenu->addAction(action);
    toolBar->addAction(action);

    action = new QAction(tr("&IDW"), this);
    action->setToolTip(tr("IDW"));
    action->setStatusTip(tr("IDW warping"));
    connect(action, &QAction::triggered, m_imageWidget.get(), &ImageWidget::actionIDW);
    editMenu->addAction(action);
    toolBar->addAction(action);

    action = new QAction(tr("&RBF"), this);
    action->setToolTip(tr("IDW"));
    action->setStatusTip(tr("IDW"));
    connect(action, &QAction::triggered, m_imageWidget.get(), &ImageWidget::actionRBF);
    editMenu->addAction(action);
    toolBar->addAction(action);

    action = new QAction(tr("&Origin"), this);
    action->setToolTip(tr("Origin"));
    action->setStatusTip(tr("Origin"));
    connect(action, &QAction::triggered, m_imageWidget.get(), &ImageWidget::actionOrigin);
    editMenu->addAction(action);
    toolBar->addAction(action);

    action = new QAction(tr("&Line"), this);
    action->setToolTip(tr("Line"));
    action->setStatusTip(tr("Line"));
    connect(action, &QAction::triggered, m_imageWidget.get(), &ImageWidget::actionLine);
    editMenu->addAction(action);
    toolBar->addAction(action);

    action = new QAction(tr("&Rect"), this);
    action->setToolTip(tr("Rect"));
    action->setStatusTip(tr("Rect"));
    connect(action, &QAction::triggered, m_imageWidget.get(), &ImageWidget::actionRect);
    editMenu->addAction(action);
    toolBar->addAction(action);

    action = new QAction(tr("&帮助"), this);
    action->setToolTip(tr("帮助"));
    action->setStatusTip(tr("帮助"));
    connect(action, &QAction::triggered, m_imageWidget.get(), &ImageWidget::actionHelp);
    helpMenu->addAction(action);

    action = new QAction(tr("&关于我"), this);
    action->setToolTip(tr("about me"));
    action->setStatusTip(tr("about me"));
    connect(action, &QAction::triggered, m_imageWidget.get(), &ImageWidget::actionAbout);
    helpMenu->addAction(action);

    menuBar()->addMenu(fileMenu)->setText(tr("&File"));
    menuBar()->addMenu(editMenu)->setText(tr("&Edit"));
    menuBar()->addMenu(helpMenu)->setText(tr("&Help"));

    statusBar();
}
MainWindow::~MainWindow() = default;
