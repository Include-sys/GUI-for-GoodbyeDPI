#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSysInfo>
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

    void onDefaultParamCheckState(Qt::CheckState state);

    QStringList prepareParameters(bool isComboParametreEnabled);

    void catchError(QProcess::ProcessError err);

private:
    Ui::MainWindow *ui;

    QSysInfo *info;
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

    enum ParameterList{
        russia_blacklist,
        russia_blacklist_dnsredir,
        all,
        all_dnsredir,
        all_dnsredir_hardcore
    };
};

#endif // MAINWINDOW_H
