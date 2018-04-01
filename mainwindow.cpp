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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("GoodByeDPI için GUI");

    QSettings settings("HexOpenSource", "GBDPI-GUI");

    connect(ui->actionAyarlar, &QAction::triggered, this, &MainWindow::onActionAyarlar);


    tmpDir = new QTemporaryDir();

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
    ayarlar.close();
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
    }
    else if(proc->state() == QProcess::Running)
    {
        ui->debugArea->appendPlainText("[+] Başlatıldı\n[+] PID:" + QString::number(proc->processId()));
        ui->btnStart->setEnabled(false);
        ui->btnStop->setEnabled(true);
    }
}

void MainWindow::onActionAyarlar()
{
    ayarlar.setWindowModality(Qt::WindowModal);
    ayarlar.show();
}
