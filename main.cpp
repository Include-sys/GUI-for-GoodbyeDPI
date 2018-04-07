#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(a.arguments());

    //qDebug() << a.arguments().isEmpty();
    bool status = false;
    //status = a.arguments().at(1) != "-silent";
    w.show();

    return a.exec();
}
