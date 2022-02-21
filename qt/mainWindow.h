//
// Created by william on 2022/2/19.
//

#ifndef CPP_DEMO_MAINWINDOW_H
#define CPP_DEMO_MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

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

private:
    void createButtons();
    void creatPushButtons();

private slots:
    /// 在这个区内声明的槽意味着只有类自己可以将信号与之相连接。这适用于联系非常紧密的类
    void test();
    void clickedTest();

private:
    Ui::MainWindow* ui;

    QAction* m_actionTest;
    QMenu* m_menuTest;
    QToolBar* m_toolbarTest;
    QPushButton* m_pushButtonTest;
};

#endif //CPP_DEMO_MAINWINDOW_H
