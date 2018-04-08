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
    ayarlar(new Settings()),
    settings(new QSettings("HexOpenSource", "GBDPI-GUI", this)),
    trayIcon(new QSystemTrayIcon(this)),
    trayMenu(new QMenu(this)),
    hideAction(new QAction("Gizle", this)),
    showAction(new QAction("Göster", this)),
    closeAction(new QAction("Çıkış", this)),
    startAction(new QAction(QIcon(":/images/images/play-button.png"), "Başlat", this)),
    stopAction(new QAction(QIcon(":/images/images/stop-button.png"), "Durdur", this)),
    settingsAction(new QAction(QIcon(":/images/images/settings-gears-button.png"), "Ayarlar", this)),
    tmpDir(new QTemporaryDir()),
    proc(new QProcess(this)),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("GoodByeDPI için GUI");
    setWindowIcon(QIcon(":/images/images/icon.ico"));

    trayIcon->setIcon(QIcon(":/images/images/icon.ico"));
    trayIcon->setToolTip("GUIForGoodByeDPI by hex4d0r");

    ui->labelParameters->setWordWrap(true);

    //For using lambda functions with traymenu
    auto& traymn = trayMenu;

    //Setting traymenu actions.
    connect(startAction, &QAction::triggered, this, &MainWindow::procStart);
    connect(stopAction, &QAction::triggered, this, &MainWindow::procStop);
    connect(closeAction, &QAction::triggered, [this](){
        QCoreApplication::quit();
    });
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
    //Set false Stop and Hide actions
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
    connect(ayarlar, &Settings::defaultParamStateChanged, this, &MainWindow::onDefaultParamCheckState, Qt::QueuedConnection);

    //PARAMETRE DUZENLE SILENT parametresini unutma procstart calistiginda oku parametreleri
    connect(ui->btnStart, &QPushButton::clicked, this, &MainWindow::procStart);
    connect(ui->btnStop, &QPushButton::clicked, this, &MainWindow::procStop);
    connect(proc, &QProcess::stateChanged, this, &MainWindow::handleState);

    ui->comboParametre->addItem("russia_blacklist", QVariant("-1 --blacklist blacklist.txt"));
    ui->comboParametre->addItem("russia_blacklist_dnsredir", QVariant("-1 --dns-addr 77.88.8.8 --dns-port 1253 --dnsv6-addr 2a02:6b8::feed:0ff --dnsv6-port 1253 --blacklist blacklist.txt"));
    ui->comboParametre->addItem("all", QVariant("-1"));
    ui->comboParametre->addItem("all_dnsredir", QVariant("-1 --dns-addr 77.88.8.8 --dns-port 1253 --dnsv6-addr 2a02:6b8::feed:0ff --dnsv6-port 1253"));
    ui->comboParametre->addItem("all_dnsredir_hardcore", QVariant("-1 -a -m --dns-addr 77.88.8.8 --dns-port 1253 --dnsv6-addr 2a02:6b8::feed:0ff --dnsv6-port 1253"));

    ui->btnStop->setEnabled(false);

    connect(ui->comboParametre, QOverload<int>::of(&QComboBox::currentIndexChanged), [this]()
    {
        prepareParameters(true);
    });

    //Capturing ouput of goodbyedpi.exe but doesn't work, because goodbyedpi.exe doesn't flush stdout. Author need to add fflush(stdout) to aftery every printf() function.
    connect(proc, &QProcess::readyReadStandardOutput, this, &MainWindow::processOutput);
    connect(proc, &QProcess::readyReadStandardError, this, &MainWindow::processError);

    if(settings->value("Parametre/defaultParam").toBool())
        prepareParameters(true);
    else
        prepareParameters(false);

    if(!this->isVisible())
    {
        hideAction->setEnabled(false);
        showAction->setEnabled(true);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    proc->kill();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //qDebug() << settings->value("System/systemTray").toString();
    if(settings->value("System/systemTray").toString() == "true" && (this->isTopLevel() || this->isVisible()))
    {
        event->ignore();
        this->hide();
        trayMenu->actions().at(4)->setEnabled(true);
        trayMenu->actions().at(5)->setEnabled(false);

        if(!settings->value("System/disableNotifications").toBool())
        {
            qDebug() << "Message will shown";
            QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
            trayIcon->showMessage("GoodByeDPI GUI", "Arka planda çalışıyor.", icon, 4000);
        }
    }
    else
    {
        ayarlar->close();
    }
}

void MainWindow::procStart()
{
    proc->setArguments(prepareParameters(ui->comboParametre->isEnabled()));
    ui->debugArea->appendPlainText("[*] " + ui->comboParametre->currentText());
    proc->start(QDir::currentPath() + "/goodbyedpi/goodbyedpi.exe", QProcess::ReadWrite);
    proc->waitForStarted(1000);

    ui->debugArea->appendPlainText(proc->errorString());

}

void MainWindow::procStop()
{
    proc->close();
    proc->waitForFinished(2000);
}

void MainWindow::processOutput()
{
    proc->setReadChannel(QProcess::StandardOutput);
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
    proc->setReadChannel(QProcess::StandardError);
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
    ui->debugArea->appendPlainText(proc->errorString());
}

void MainWindow::onActionAyarlar()
{
    ayarlar->show();
}

void MainWindow::onActionAbout()
{
    hakkinda.exec();
}

void MainWindow::onDefaultParamCheckState(Qt::CheckState state)
{
    if(state == Qt::Checked)
    {
        ui->comboParametre->setEnabled(true);
        prepareParameters(true);
    }
    else
    {
       ui->comboParametre->setEnabled(false);
       prepareParameters(false);
    }

}

QStringList MainWindow::prepareParameters(bool isComboParametreEnabled)
{
    QStringList defaultparameters;
    QStringList customParameters;
    QStringList quickParameters;
    QStringList param2Box;
    QStringList param1Box;

    //PARAMBOX1
    if(settings->value("Parametre/paramP").toBool())
        param1Box << "-p";
    if(settings->value("Parametre/paramR").toBool())
        param1Box << "-r";
    if(settings->value("Parametre/paramS").toBool())
        param1Box << "-s";
    if(settings->value("Parametre/paramM").toBool())
        param1Box << "-m";
    if(settings->value("Parametre/paramF").toString() != "false")
        param1Box << settings->value("Parametre/paramF").toString();
    if(settings->value("Parametre/paramK").toString() != "false")
        param1Box << settings->value("Parametre/paramK").toString();
    if(settings->value("Parametre/paramN").toBool())
        param1Box << "-n";
    if(settings->value("Parametre/paramE").toString() != "false")
        param1Box << settings->value("Parametre/paramE").toString();
    if(settings->value("Parametre/paramA").toBool())
        param1Box << "-a";
    if(settings->value("Parametre/paramW").toBool())
        param1Box << "-w";
    if(settings->value("Parametre/paramPort").toString() != "false")
        param1Box << settings->value("Parametre/paramPort").toString();
    if(settings->value("Parametre/paramIpId").toString() != "false")
        param1Box << settings->value("Parametre/paramIpId").toString();

    //PARAMBOX2
    if(settings->value("Parametre/paramDnsAddr").toString() != "false")
        param2Box << settings->value("Parametre/paramDnsAddr").toString();
    if(settings->value("Parametre/paramDnsPort").toString() != "false")
        param2Box << settings->value("Parametre/paramDnsPort").toString();
    if(settings->value("Parametre/paramDnsPort").toString() != "false")
        param2Box << settings->value("Parametre/paramDnsPort").toString();
    if(settings->value("Parametre/paramDnsv6Addr").toString() != "false")
        param2Box << settings->value("Parametre/paramDnsv6Addr").toString();
    if(settings->value("Parametre/paramDnsv6Port").toString() != "false")
        param2Box << settings->value("Parametre/paramDnsv6Port").toString();
    if(settings->value("Parametre/paramBlacklist").toString() != "false")
        param2Box << "--blacklist blacklist.txt";

    //QUICKSETTINGS
    if(settings->value("Parametre/paramQuick").toString() == "-1")
        quickParameters << "-p -r -s -f 2 -k 2 -n -e 2" << param2Box;
    if(settings->value("Parametre/paramQuick").toString() == "-2")
        quickParameters << "-p -r -s -f 2 -k 2 -n -e 40" << param2Box;
    else if(settings->value("Parametre/paramQuick").toString() == "-3")
        quickParameters << "-p -r -s -e 40" << param2Box;
    else if(settings->value("Parametre/paramQuick").toString() == "-4")
        quickParameters << "-p -r -s" << param2Box;

    //DEFAULTPARAMETERS
    switch (ui->comboParametre->currentIndex()) {
    case 0:
        defaultparameters << "-1 --blacklist blacklist.txt";
        break;
    case 1:
        defaultparameters << "-1 --dns-addr 77.88.8.8 --dns-port 1253 --dnsv6-addr 2a02:6b8::feed:0ff --dnsv6-port 1253 --blacklist blacklist.txt";
        break;
    case 2:
        defaultparameters << "-1";
        break;
    case 3:
        defaultparameters << "-1 --dns-addr 77.88.8.8 --dns-port 1253 --dnsv6-addr 2a02:6b8::feed:0ff --dnsv6-port 1253";
        break;
    case 4:
        defaultparameters << "-1 -a -m --dns-addr 77.88.8.8 --dns-port 1253 --dnsv6-addr 2a02:6b8::feed:0ff --dnsv6-port 1253";
    }

    //CUSTOMPARAMETERS
    customParameters << param1Box << param2Box;

    //UPDATE Parameter Label
    if(isComboParametreEnabled)
    {
        ui->labelParameters->setText("goodbyedpi.exe " + defaultparameters.join(" "));
        return defaultparameters;
    }
    else if(settings->value("Parametre/customParam").toString() == "true" && settings->value("Parametre/quickSettings").toString() == "false")
    {
        ui->labelParameters->setText("goodbyedpi.exe " + customParameters.join(" "));
        return customParameters;
    }
    else if(settings->value("Parametre/customParam").toString() == "false" && settings->value("Parametre/quickSettings").toString() == "false")
    {
        ui->labelParameters->setText("goodbyedpi.exe");
        return QStringList();
    }
    else if(settings->value("Parametre/customParam").toString() == "true" && settings->value("Parametre/quickSettings").toString() == "true")
    {
        ui->labelParameters->setText("goodbyedpi.exe" + quickParameters.join(" "));
        return QStringList();
    }
    else
    {
        ui->labelParameters->setText("goodbyedpi.exe " + quickParameters.join(" "));
        return quickParameters;
    }

}
