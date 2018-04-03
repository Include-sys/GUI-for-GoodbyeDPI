#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDirIterator>
#include <QListIterator>
#include <memory>
#include <Windows.h>

MainWindow::MainWindow(QStringList arguments, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("GoodByeDPI için GUI");
    setWindowIcon(QIcon(":/images/images/icon.ico"));

    qDebug() << arguments.at(1);

    //Maybe I could choose better variable names :)
    ayarlar = new Settings();
    settings = new QSettings("HexOpenSource", "GBDPI-GUI");

    //Creating trayIcon and menu
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/images/images/icon.ico"));
    trayIcon->setToolTip("GUIForGoodByeDPI by hex4d0r");

    trayMenu = new QMenu(this);
    auto& traymn = trayMenu;

    hideAction = new QAction("Gizle", this);
    showAction = new QAction("Goster", this);
    closeAction = new QAction("Cikis", this);
    startAction = new QAction(QIcon(":/images/images/play-button.png"), "Baslat", this);
    stopAction = new QAction(QIcon(":/images/images/stop-button.png"), "Durdur", this);
    settingsAction = new QAction(QIcon(":/images/images/settings-gears-button.png"), "Ayarlar", this);

    connect(startAction, &QAction::triggered, this, &MainWindow::procStart);
    connect(stopAction, &QAction::triggered, this, &MainWindow::procStop);
    connect(closeAction, &QAction::triggered, this, &MainWindow::close);
    connect(hideAction, &QAction::triggered, [this, traymn](){
        this->hide();
        traymn->actions().at(5)->setEnabled(false);
        traymn->actions().at(4)->setEnabled(true);
    });
    connect(settingsAction, &QAction::triggered, this, &MainWindow::onActionAyarlar);
    connect(showAction, &QAction::triggered, [this, traymn](){
        this->show();
        traymn->actions().at(5)->setEnabled(true);
        traymn->actions().at(4)->setEnabled(false);
    });

    QList<QAction*> actionList;
    actionList << startAction << stopAction << settingsAction << showAction << hideAction << closeAction;

    trayMenu->addActions(actionList);
    trayMenu->insertSeparator(showAction);

    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    trayMenu->actions().at(4)->setEnabled(false);
    trayMenu->actions().at(1)->setEnabled(false);

    connect(ui->actionAyarlar, &QAction::triggered, this, &MainWindow::onActionAyarlar);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onActionAbout);

    //Checking if default parameters enabled or not due to enable/disable parameters combo box.
    if(!settings->value("Parametre/defaultParam").toBool())
    {
        ui->comboParametre->setEnabled(false);
    }

    //Capturing state of default parameters checkbox for enable/disable parameters combo box.
    connect(ayarlar, &Settings::defaultParamStateChanged, this, &MainWindow::onDefaultParamCheckState);

    tmpDir = new QTemporaryDir();
    //PARAMETRE DUZENLE SILENT parametresini unutma procstart calistiginda oku parametreleri
    connect(ui->btnStart, &QPushButton::clicked, this, &MainWindow::procStart);
    connect(ui->btnStop, &QPushButton::clicked, this, &MainWindow::procStop);

    args << "-1 --blacklist blacklist.txt" <<
            "-1 --dns-addr 77.88.8.8 --dns-port 1253 --dnsv6-addr 2a02:6b8::feed:0ff --dnsv6-port 1253 --blacklist blacklist.txt" <<
            "-1 --dns-addr 77.88.8.8 --dns-port 1253 --dnsv6-addr 2a02:6b8::feed:0ff --dnsv6-port 1253" <<
            "-1 -a -m --dns-addr 77.88.8.8 --dns-port 1253 --dnsv6-addr 2a02:6b8::feed:0ff --dnsv6-port 1253";

    ui->comboParametre->addItems(args);

    ui->btnStop->setEnabled(false);

    proc = new QProcess(this);

    connect(proc, &QProcess::readyReadStandardOutput, this, &MainWindow::processOutput);
    connect(proc, &QProcess::readyReadStandardError, this, &MainWindow::processError);
    connect(proc, &QProcess::stateChanged, this, &MainWindow::handleState);

    //proc->setReadChannel(QProcess::StandardOutput);

}

MainWindow::~MainWindow()
{
    delete ui;
    proc->kill();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    ayarlar->ayarKayit();
    qDebug() << settings->value("System/systemTray");
    if(settings->value("System/systemTray").toString() == "true" && this->isVisible())
    {
        event->ignore();
        this->hide();
        trayMenu->actions().at(4)->setEnabled(true);
        trayMenu->actions().at(5)->setEnabled(false);
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);

        trayIcon->showMessage("GoodByeDPI icin GUI", "Calisiyor", icon, 4000);
    }
    else
    {
        ayarlar->close();
    }
}

void MainWindow::procStart()
{
    if(proc->state() == QProcess::Running)
    {
        return;
    }
    proc->setProcessChannelMode(QProcess::MergedChannels);

    if(!tmpDir->isValid())  return;
    QString tempPath = tmpDir->path();

    QDirIterator it(":/GBDPI", QDir::Files, QDirIterator::Subdirectories);
    QList<QFileInfo> f;
    while (it.hasNext()) {
        f << it.next();
    }

    QString tmp;

    for(auto i=0; i < f.size(); i++)
    {
        tmp = tempPath + "/" + f[i].fileName();
        QFile::copy(f[i].absoluteFilePath(), tmp);
    }
    //tempPath + "/goodbyedpi.exe"
    //QProcess::execute("PUSHD " + tempPath + "/");
    proc->setArguments(QStringList() << ui->comboParametre->currentText());
    ui->debugArea->appendPlainText("[*] " + ui->comboParametre->currentText());
    proc->start(tempPath + "/goodbyedpi.exe", QProcess::Unbuffered | QProcess::ReadWrite);
    proc->waitForStarted(1000);

    qDebug() << proc->errorString();

}

void MainWindow::procStop()
{
    proc->kill();
    proc->waitForFinished(2000);
}

void MainWindow::processOutput()
{
    proc->setReadChannel(QProcess::StandardOutput);
    proc->write(" ");
    QString output = proc->readAllStandardOutput();

    if(!output.isEmpty())
    {
        ui->debugArea->appendPlainText(output);
    }
    else
    {
        ui->debugArea->appendPlainText(proc->errorString());
    }
}

void MainWindow::processError()
{
    QString errout = proc->readAllStandardError();

    if(!errout.isEmpty())
    {
        ui->debugArea->appendPlainText(errout);

    }
    else
    {
        ui->debugArea->appendPlainText(proc->errorString());
    }
}
void MainWindow::handleState()
{
    if(proc->state() == QProcess::NotRunning)
    {
        ui->debugArea->appendPlainText("[-] Durduruldu");
        ui->btnStart->setEnabled(true);
        ui->btnStop->setEnabled(false);
        trayMenu->actions().at(1)->setEnabled(false);
        trayMenu->actions().at(0)->setEnabled(true);
    }
    else if(proc->state() == QProcess::Running)
    {
        ui->debugArea->appendPlainText("[+] Başlatıldı\n[+] PID:" + QString::number(proc->processId()));
        ui->btnStart->setEnabled(false);
        ui->btnStop->setEnabled(true);
        trayMenu->actions().at(0)->setEnabled(false);
        trayMenu->actions().at(1)->setEnabled(true);
    }
}

void MainWindow::onActionAyarlar()
{
    ayarlar->setWindowModality(Qt::WindowModal);
    ayarlar->show();
}

void MainWindow::onActionAbout()
{
    hakkinda.exec();
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{

}

void MainWindow::onDefaultParamCheckState(Qt::CheckState state)
{
    if(state == Qt::Checked)
    {
        ui->comboParametre->setEnabled(true);
    }
    else
    {
        ui->comboParametre->setEnabled(false);
    }

}
