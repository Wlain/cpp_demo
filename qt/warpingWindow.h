//
// Created by william on 2022/2/19.
//

#ifndef CPP_DEMO_WARPINGWINDOW_H
#define CPP_DEMO_WARPINGWINDOW_H

#include "ui_MainWindow.h"

class MainView;
class MainScene;

class WarpingWindow : public QMainWindow, public Ui::WarpingWindow
{
    Q_OBJECT

public:
    explicit WarpingWindow(QWidget* parent = nullptr);
    ~WarpingWindow() override;
//    void paintEvent(QPaintEvent* event) override;

private slots:
    /// 在这个区内声明的槽意味着只有类自己可以将信号与之相连接。这适用于联系非常紧密的类
    void on_actionnew_triggered();
    void on_actionopen_triggered();
    void on_actionsave_triggered();
    void on_actionclose_triggered();
    void on_actionundo_triggered();
    void on_actionrevert_triggered();
    void on_actiongray_triggered();
    void on_actionIDW_triggered();
    void on_actionRBF_triggered();

private:
    std::unique_ptr<MainView> m_mainView;
    std::unique_ptr<MainScene> m_scene;
};

#endif //CPP_DEMO_WARPINGWINDOW_H
