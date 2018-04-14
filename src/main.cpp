#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QStringList>
#include <QTranslator>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString systemLang = QLocale::languageToString(QLocale::system().language());

    QTranslator t;

    if(systemLang != "Turkish")
    {
        t.load(":/english.qm");
        a.installTranslator(&t);
    }

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
