//
// Created by cwb on 2022/2/9.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Viewwidget.h" resolved

#include "viewwidget.h"

#include "ui_wiewwidget.h"

ViewWidget::ViewWidget(QWidget* parent) :
    QWidget(parent), ui(new Ui::ViewWidget)
{
    ui->setupUi(this);
}

ViewWidget::~ViewWidget()
{
    delete ui;
}
void ViewWidget::setLine()
{
}
void ViewWidget::setRect()
{
}
void ViewWidget::setEllipse()
{
}
void ViewWidget::setPoly()
{
}
void ViewWidget::setFreeHand()
{
}
void ViewWidget::setColor(QColor c)
{
}
void ViewWidget::setWidth(int w)
{
}
void ViewWidget::setBackground()
{
}
void ViewWidget::undo()
{
}
void ViewWidget::redo()
{
}
void ViewWidget::clear()
{
}
