//
// Created by cwb on 2022/2/9.
//

#ifndef QT_DEMO_MAINWINDOW_H
#define QT_DEMO_MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;


private slots:
    void helloWorld();

private:
    Ui::MainWindow * m_ui;
    QAction* m_action;
    QMenu* m_meum;
    QToolBar* m_toolbar;
};


#endif //QT_DEMO_MAINWINDOW_H
