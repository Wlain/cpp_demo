//
// Created by cwb on 2022/2/24.
//
/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QmenuBar>
#include <QtWidgets/QstatusBar>
#include <QtWidgets/QtoolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
class WarpingWindow
{
public:
    WarpingWindow();
    virtual ~WarpingWindow();

public:
    void setupUi(QMainWindow* mainWindow);

private:
    void retranslateUi(QMainWindow* WarpingWindow);

public:
    QAction* m_actionNew;
    QAction* m_actionOpen;
    QAction* m_actionSave;
    QAction* m_actionClose;
    QAction* m_actionIDW;
    QAction* m_actionRBF;
    QAction* m_actionUndo;
    QAction* m_actionRevert;
    QAction* m_actionGray;
    QAction* m_actionMirrorH;
    QAction* m_actionMirrorV;
    QAction* m_actionOrigin;
    QWidget* m_centralWidget;
    QMenuBar* m_menuBar;
    QMenu* m_menuFile;
    QMenu* m_menuEdit;
    QStatusBar* m_statusBar;
    QToolBar* m_toolBar;
};

} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
