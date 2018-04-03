#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTemporaryDir>
#include <QProcess>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QMenu>
#include "settings.h"
#include "about.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QStringList, QWidget *parent = 0);
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
    void onActionAbout();

    void iconActivated(QSystemTrayIcon::ActivationReason reason);

    void onDefaultParamCheckState(Qt::CheckState state);

private:
    Ui::MainWindow *ui;

    QTemporaryDir *tmpDir;
    QSettings *settings;

    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;

    QAction *hideAction;
    QAction *showAction;
    QAction *closeAction;
    QAction *startAction;
    QAction *stopAction;
    QAction *settingsAction;

    QProcess *proc;

    QStringList args;

    Settings *ayarlar;
    About hakkinda;
};

#endif // MAINWINDOW_H
