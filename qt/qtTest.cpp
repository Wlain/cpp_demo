//
// Created by william on 2022/2/19.
//

#include <QApplication>
#include "mainWindow.h"

int qtTest(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
