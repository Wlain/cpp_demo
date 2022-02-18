//
// Created by cwb on 2022/2/18.
//

#include <QApplication>
#include "mainWindow.h"

int qtTest(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(1024, 768);
    w.setWindowTitle("mini Draw");
    w.show();
    return a.exec();
}
