//
// Created by william on 2022/2/19.
//

#include "mainWindow.h"

#include <QApplication>

namespace qtTest
{
int qtTest(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}
} // namespace qtTest
