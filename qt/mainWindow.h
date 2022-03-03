//
// Created by william on 2022/2/19.
//

#ifndef CPP_DEMO_MAINWINDOW_H
#define CPP_DEMO_MAINWINDOW_H

#include <QMainWindow>

class ImageWidget;

class MainWindow : public QMainWindow
{
public:
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    std::unique_ptr<ImageWidget> m_imageWidget;
};

#endif //CPP_DEMO_MAINWINDOW_H
