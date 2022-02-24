//
// Created by william on 2022/2/19.
//

#ifndef CPP_DEMO_WARPINGWINDOW_H
#define CPP_DEMO_WARPINGWINDOW_H

#include "warpingWindowUI.h"

class MainView;
class MainScene;

class WarpingWindow : public QMainWindow, public Ui::WarpingWindow
{
public:
    Q_OBJECT
public:
    explicit WarpingWindow(QWidget* parent = nullptr);
    ~WarpingWindow() override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    /// 在这个区内声明的槽意味着只有类自己可以将信号与之相连接。这适用于联系非常紧密的类
    void actionNew();
    void actionOpen();
    void actionSave();
    void actionClose();
    void actionUndo();
    void actionRevert();
    void actionGray();
    void actionIDW();
    void actionRBF();
    void actionMirrorH();
    void actionMirrorV();
    void actionOrigin();

private:
    std::unique_ptr<MainView> m_mainView;
    std::unique_ptr<MainScene> m_scene;
};

#endif //CPP_DEMO_WARPINGWINDOW_H
