#include <QApplication>
#include "mainwindow.h"
int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(1024, 768);
    w.setWindowTitle("mini Draw");
    w.setBackgroundRole(QPalette::Background)
    w.show();
    return a.exec();
}
