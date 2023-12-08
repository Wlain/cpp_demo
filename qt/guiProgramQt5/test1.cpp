//
// Created by william on 2023/12/8.
//

#include <QApplication>

namespace qtTest
{
int qtTest1(int argc, char* argv[])
{
    QApplication a(argc, argv);
    return QApplication::exec();
}
} // namespace qtTest
