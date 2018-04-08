#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QStringList>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(a.arguments());


    QStringList argList = a.arguments();
    QString argument = argList.join(",");

    bool silent = argument.contains("-silent");

    if(!silent)
    {
        w.show();
    }


    return a.exec();
}
