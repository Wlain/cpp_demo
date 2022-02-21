//
// Created by william on 2022/2/19.
//

#ifndef CPP_DEMO_MAINWINDOW_H
#define CPP_DEMO_MAINWINDOW_H

#include <QCheckBox>
#include <QMainWindow>
#include <QPushButton>
#include <QRadioButton>

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
    void createRadioButton();
    void creatCheckBox();

private slots:
    /// 在这个区内声明的槽意味着只有类自己可以将信号与之相连接。这适用于联系非常紧密的类
    void test();
    void clickedTest();
    void radioButtonTest();
    void checkBoxTest();

private:
    Ui::MainWindow* ui;
    QAction* m_actionTest = nullptr;
    QMenu* m_menuTest = nullptr;
    QToolBar* m_toolbarTest = nullptr;
    QPushButton* m_pushButtonTest = nullptr;
    QRadioButton* m_radioButtonTest = nullptr;
    QCheckBox* m_checkBoxTest = nullptr;
};

#endif //CPP_DEMO_MAINWINDOW_H
