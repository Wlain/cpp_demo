//
// Created by cwb on 2022/2/9.
//

#ifndef QT_DEMO_MAINWINDOW_H
#define QT_DEMO_MAINWINDOW_H

#include "viewwidget.h"
#include <QLabel>
#include <QMainWindow>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void helloWorld();
    void creatMenu();
    void createToolBar();
    void createAction();

    void aboutBox();

private:
    Ui::MainWindow* m_ui;
    ViewWidget* m_view;

    QMenu* m_fileMenu = nullptr;
    QMenu* m_figureMenu = nullptr;
    QMenu* m_editMenu = nullptr;

    QToolBar* m_toolBar = nullptr;
    QToolBar* m_toolBar2 = nullptr;
    QAction* m_aboutAction;
    QAction* m_lineAction;
    QAction* m_rectAction;
    QAction* m_ellipseAction;
    QAction* m_polyAction;
    QAction* m_freeHandAction;
    QAction* m_colorAction;
    QAction* m_widthAction;
    QAction* m_openAction;
    QAction* m_undoAction;
    QAction* m_redoAction;
    QAction* m_clearAction;

    QAction* m_action;
    QMenu* m_meum;
    QToolBar* m_toolbar;
    QLabel* m_label;
};

#endif //QT_DEMO_MAINWINDOW_H
