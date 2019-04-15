#include "mainwindow.h"
#include <QApplication>
#include "tests.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    tests test;
    MainWindow w;
    w.show();
    return a.exec();
}
