//
// Created by cwb on 2022/2/24.
//

#include <warpingWindowUI.h>

namespace Ui
{
WarpingWindow::WarpingWindow() = default;

WarpingWindow::~WarpingWindow() = default;

void WarpingWindow::setupUi(QMainWindow* mainWindow)
{
    if (mainWindow->objectName().isEmpty())
    {
        mainWindow->setObjectName(QString::fromUtf8("mainWindow"));
    }
    mainWindow->resize(720, 720);

    m_actionNew = new QAction(mainWindow);
    m_actionNew->setObjectName(QString::fromUtf8("m_actionNew"));

    QIcon icon;
    icon.addFile(QString::fromUtf8("../resources/icons/filenew.png"), QSize(), QIcon::Normal, QIcon::Off);
    m_actionNew->setIcon(icon);
    m_actionOpen = new QAction(mainWindow);
    m_actionOpen->setObjectName(QString::fromUtf8("m_actionOpen"));

    QIcon icon1;
    icon1.addFile(QString::fromUtf8("../resources/icons/fileopen.png"), QSize(), QIcon::Normal, QIcon::Off);
    m_actionOpen->setIcon(icon1);
    m_actionSave = new QAction(mainWindow);
    m_actionSave->setObjectName(QString::fromUtf8("m_actionSave"));

    QIcon icon2;
    icon2.addFile(QString::fromUtf8("../resources/icons/filesave.png"), QSize(), QIcon::Normal, QIcon::Off);
    m_actionSave->setIcon(icon2);
    m_actionClose = new QAction(mainWindow);
    m_actionClose->setObjectName(QString::fromUtf8("m_actionClose"));

    QIcon icon3;
    icon3.addFile(QString::fromUtf8("../resources/icons/fileclose.png"), QSize(), QIcon::Normal, QIcon::Off);
    m_actionClose->setIcon(icon3);
    m_actionIDW = new QAction(mainWindow);
    m_actionIDW->setObjectName(QString::fromUtf8("m_actionIDW"));
    m_actionRBF = new QAction(mainWindow);
    m_actionRBF->setObjectName(QString::fromUtf8("m_actionRBF"));
    m_actionUndo = new QAction(mainWindow);
    m_actionUndo->setObjectName(QString::fromUtf8("m_actionUndo"));

    QIcon icon4;
    icon4.addFile(QString::fromUtf8("../resources/icons/undo.png"), QSize(), QIcon::Normal, QIcon::Off);
    m_actionUndo->setIcon(icon4);
    m_actionRevert = new QAction(mainWindow);
    m_actionRevert->setObjectName(QString::fromUtf8("m_actionRevert"));
    m_actionGray = new QAction(mainWindow);
    m_actionGray->setObjectName(QString::fromUtf8("m_actionGray"));
    m_actionMirrorH = new QAction(mainWindow);
    m_actionMirrorH->setObjectName(QString::fromUtf8("m_actionMirrorH"));
    m_actionMirrorV = new QAction(mainWindow);
    m_actionMirrorV->setObjectName(QString::fromUtf8("m_actionMirrorV"));
    m_actionOrigin = new QAction(mainWindow);
    m_actionOrigin->setObjectName(QString::fromUtf8("m_actionOrigin"));

    m_centralWidget = new QWidget(mainWindow);
    m_centralWidget->setObjectName(QString::fromUtf8("m_centralWidget"));
    mainWindow->setCentralWidget(m_centralWidget);
    m_menuBar = new QMenuBar(mainWindow);
    m_menuBar->setObjectName(QString::fromUtf8("m_menuBar"));
    m_menuBar->setGeometry(QRect(0, 0, 720, 24));
    m_menuFile = new QMenu(m_menuBar);
    m_menuFile->setObjectName(QString::fromUtf8("m_menuFile"));
    m_menuEdit = new QMenu(m_menuBar);
    m_menuEdit->setObjectName(QString::fromUtf8("m_menuEdit"));
    mainWindow->setMenuBar(m_menuBar);
    m_statusBar = new QStatusBar(mainWindow);
    m_statusBar->setObjectName(QString::fromUtf8("m_statusBar"));
    mainWindow->setStatusBar(m_statusBar);
    m_toolBar = new QToolBar(mainWindow);
    m_toolBar->setObjectName(QString::fromUtf8("m_toolBar"));
    mainWindow->addToolBar(Qt::TopToolBarArea, m_toolBar);

    m_menuBar->addAction(m_menuFile->menuAction());
    m_menuBar->addAction(m_menuEdit->menuAction());
    m_menuFile->addAction(m_actionNew);
    m_menuFile->addAction(m_actionOpen);
    m_menuFile->addAction(m_actionSave);
    m_menuFile->addAction(m_actionClose);
    m_menuEdit->addAction(m_actionIDW);
    m_menuEdit->addAction(m_actionRBF);
    m_toolBar->addAction(m_actionNew);
    m_toolBar->addAction(m_actionOpen);
    m_toolBar->addAction(m_actionSave);
    m_toolBar->addAction(m_actionClose);
    m_toolBar->addAction(m_actionUndo);
    m_toolBar->addAction(m_actionRevert);
    m_toolBar->addAction(m_actionGray);
    m_toolBar->addAction(m_actionMirrorH);
    m_toolBar->addAction(m_actionMirrorV);
    m_toolBar->addAction(m_actionIDW);
    m_toolBar->addAction(m_actionRBF);
    m_toolBar->addAction(m_actionOrigin);

    retranslateUi(mainWindow);
    QMetaObject::connectSlotsByName(mainWindow);
} // setupUi

void WarpingWindow::retranslateUi(QMainWindow* WarpingWindow)
{
    WarpingWindow->setWindowTitle(QCoreApplication::translate("WarpingWindow", "WarpingWindow", nullptr));
    m_actionNew->setText(QCoreApplication::translate("WarpingWindow", "new", nullptr));
    m_actionOpen->setText(QCoreApplication::translate("WarpingWindow", "open", nullptr));
    m_actionSave->setText(QCoreApplication::translate("WarpingWindow", "save", nullptr));
    m_actionClose->setText(QCoreApplication::translate("WarpingWindow", "close", nullptr));
    m_actionIDW->setText(QCoreApplication::translate("WarpingWindow", "IDW", nullptr));
    m_actionRBF->setText(QCoreApplication::translate("WarpingWindow", "RBF", nullptr));
    m_actionUndo->setText(QCoreApplication::translate("WarpingWindow", "undo", nullptr));
    m_actionRevert->setText(QCoreApplication::translate("WarpingWindow", "revert", nullptr));
    m_actionMirrorH->setText(QCoreApplication::translate("WarpingWindow", "mirrorH", nullptr));
    m_actionMirrorV->setText(QCoreApplication::translate("WarpingWindow", "mirrorV", nullptr));
    m_actionOrigin->setText(QCoreApplication::translate("WarpingWindow", "origin", nullptr));
    m_actionGray->setText(QCoreApplication::translate("WarpingWindow", "gray", nullptr));
    m_menuFile->setTitle(QCoreApplication::translate("WarpingWindow", "File", nullptr));
    m_menuEdit->setTitle(QCoreApplication::translate("WarpingWindow", "Edit", nullptr));
    m_toolBar->setWindowTitle(QCoreApplication::translate("WarpingWindow", "m_toolBar", nullptr));
} // retranslateUi

} // namespace Ui