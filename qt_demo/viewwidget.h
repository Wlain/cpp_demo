//
// Created by cwb on 2022/2/9.
//

#ifndef QT_DEMO_VIEWWIDGET_H
#define QT_DEMO_VIEWWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
class ViewWidget;
}
QT_END_NAMESPACE

class ViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ViewWidget(QWidget* parent = nullptr);
    ~ViewWidget() override;

    void setLine();
    void setRect();
    void setEllipse();
    void setPoly();
    void setFreeHand();

    void setColor(QColor c);
    void setWidth(int w);
    void setBackground();

    void undo();
    void redo();
    void clear();

private:
    Ui::ViewWidget* ui;
};

#endif //QT_DEMO_VIEWWIDGET_H
