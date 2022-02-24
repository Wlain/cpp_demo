//
// Created by cwb on 2022/2/23.
//
#include <QApplication>
#include "warpingWindow.h"

int imageWarping(int argc, char* argv[])
{
    QApplication a(argc, argv);
    WarpingWindow w;
    w.show();
    return a.exec();
}