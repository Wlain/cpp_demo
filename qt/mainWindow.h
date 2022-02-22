//
// Created by william on 2022/2/19.
//

#ifndef CPP_DEMO_MAINWINDOW_H
#define CPP_DEMO_MAINWINDOW_H

#include "ui_MainWindow.h"

class MainView;

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
    void paintEvent(QPaintEvent* event) override;

private slots:
    /// 在这个区内声明的槽意味着只有类自己可以将信号与之相连接。这适用于联系非常紧密的类
    void on_actionnew_triggered();
    void on_actionopen_triggered();
    void on_actionsave_triggered();
    void on_actionclose_triggered();
    void on_actionundo_triggered();
    void on_actionIDW_triggered();
    void on_actionRBF_triggered();

private:
    MainView* m_mainView = nullptr;
};

#endif //CPP_DEMO_MAINWINDOW_H
