#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTemporaryDir>
#include <QProcess>
#include <QSettings>
#include <QSystemTrayIcon>
#include "settings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void procStart();
    void procStop();

    void processOutput();
    void processError();
    void handleState();

    void onActionAyarlar();

private:
    Ui::MainWindow *ui;

    QTemporaryDir *tmpDir;

    QSystemTrayIcon trayIcon;
    QMenu *trayMenu;

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *quitAction;
    QAction *restoreAction;

    QProcess *proc;

    QStringList args;

    Settings ayarlar;
};

#endif // MAINWINDOW_H
