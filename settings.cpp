#include "settings.h"
#include "ui_settings.h"

#include <QDebug>

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

    QSettings *ayar = new QSettings("HexOpenSource", "GBDPI-GUI");

    ayar->beginGroup("Parametre");
    ayar->setValue("paramP", "-p");
    ayar->setValue("paramR", "-r");
    ayar->setValue("paramS", "-s");
    ayar->setValue("paramF", "-f " + QString::number(spinFValue));
    ayar->setValue("paramK", "-k " + QString::number(spinKValue));
    ayar->setValue("paramN", "-n");
    ayar->setValue("paramE", "-e " + QString::number(spinEValue));
    ayar->setValue("paramA", "-a");
    ayar->setValue("paramW", "-w");
    ayar->setValue("paramPort", "--port " + QString::number(spinPortValue));
    ayar->setValue("paramIpId", "--ip-id " + lineIpIdValue);
    ayar->setValue("paramDnsAddr", "--dns-addr " + lineDnsAddrValue);
    ayar->setValue("paramDnsPort", "--dns-port " + QString::number(spinDnsPortValue));
    ayar->setValue("paramDnsv6Addr", "--dnsv6-addr " + lineDnsv6AddrValue);
    ayar->setValue("paramDnsv6Port", "--dnsv6-port " + QString::number(spinDnsv6PortValue));
    ayar->setValue("paramBlacklist", "--blacklist blacklist.txt");
    ayar->endGroup();

    ui->paramBox->setEnabled(false);
    ui->groupQuickSettings->setEnabled(false);

    connect(ui->checkDefaultParam, &QCheckBox::clicked, this, &Settings::onCheckedDefaultParam);
    connect(ui->checkCustomParam, &QCheckBox::clicked, this, &Settings::onCheckedCustomParam);
    connect(ui->checkQuickSettings, &QCheckBox::clicked, this, &Settings::onCheckedQuickSettings);

    connect(ui->spinF, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this, ayar](){
        if(ui->checkF->isChecked()){
            spinFValue = ui->spinF->value();
            ayar->setValue("Parametre/paramF", "-f " + QString::number(spinFValue));
        }
    });
    connect(ui->spinK, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](){
        if(ui->checkK->isChecked()){
            spinKValue = ui->spinK->value();
            ayar->setValue("Parametre/paramK", "-k " + QString::number(spinKValue));
        }
    });
    connect(ui->spinPort, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](){
        if(ui->checkPort->isChecked())
        {
            spinPortValue = ui->spinPort->value();
            ayar->setValue("Parametre/paramPort", "--port " + QString::number(spinPortValue));
        }
    });
    connect(ui->spinDnsPort, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](){
        if(ui->checkDnsPort->isChecked())
        {
            spinDnsPortValue = ui->spinDnsPort->value();
            ayar->setValue("Parametre/paramDnsPort", "--dns-port " + QString::number(spinDnsPortValue));
        }
    });
    connect(ui->spinDnsv6Port, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](){
        if(ui->checkDnsv6Port->isChecked()) spinDnsv6PortValue = ui->spinDnsv6Port->value();
    });

    connect(ui->lineIpId, &QLineEdit::textChanged, [this](){
        if(ui->checkIpId->isChecked()) lineIpIdValue = ui->lineIpId->text();
    });
    connect(ui->lineDnsAddr, &QLineEdit::textChanged, [this](){
        if(ui->checkDnsAddr->isChecked()) lineDnsAddrValue = ui->lineDnsAddr->text();
    });
    connect(ui->lineDnsv6Addr, &QLineEdit::textChanged, [this](){
        if(ui->checkDnsv6Addr->isChecked()) lineDnsv6AddrValue = ui->lineDnsv6Addr->text();
    });

    connect(ui->checkP, &QCheckBox::clicked, this, &Settings::onCheckedP);
    connect(ui->checkR, &QCheckBox::clicked, this, &Settings::onCheckedR);
    connect(ui->checkS, &QCheckBox::clicked, this, &Settings::onCheckedS);
    connect(ui->checkM, &QCheckBox::clicked, this, &Settings::onCheckedM);
    connect(ui->checkF, &QCheckBox::clicked, this, &Settings::onCheckedF);
    connect(ui->checkK, &QCheckBox::clicked, this, &Settings::onCheckedK);
    connect(ui->checkN, &QCheckBox::clicked, this, &Settings::onCheckedN);
    connect(ui->checkE, &QCheckBox::clicked, this, &Settings::onCheckedE);
    connect(ui->checkA, &QCheckBox::clicked, this, &Settings::onCheckedA);
    connect(ui->checkW, &QCheckBox::clicked, this, &Settings::onCheckedW);
    connect(ui->checkPort, &QCheckBox::clicked, this, &Settings::onCheckedPort);
    connect(ui->checkIpId, &QCheckBox::clicked, this, &Settings::onCheckedIpId);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::onCheckedDefaultParam()
{
    if(ui->checkDefaultParam->isChecked())
    {
        ui->paramBox->setEnabled(false);
        ui->checkCustomParam->setCheckState(Qt::Unchecked);
    }
    else
    {

    }

    //QSettings ayarlari yapilacak
}

void Settings::onCheckedCustomParam()
{
    if(ui->checkCustomParam->isChecked())
    {
        ui->checkDefaultParam->setChecked(Qt::Unchecked);
        ui->paramBox->setEnabled(true);
    }
    else
    {
        ui->paramBox->setEnabled(false);
    }

    //QSettings ayarlari yapilacak
}

void Settings::onCheckedSystemTray()
{
    //QSettings ayarlari yapilacak
}

void Settings::onCheckedStartup()
{
    //QSettings ayarlari yapilacak
}

void Settings::onCheckedSchedule()
{
    if(ui->scheduleBox->isChecked())
    {

    }
    else
    {

    }
}

void Settings::onCheckedQuickSettings()
{
    if(ui->checkQuickSettings->isChecked())
    {
        ui->groupQuickSettings->setEnabled(true);
    }
    else
    {
        ui->groupQuickSettings->setEnabled(false);
    }
}

void Settings::onCheckedP()
{
    if(ui->checkP->isChecked())
    {

    }
}

void Settings::onCheckedR()
{
    if(ui->checkP->isChecked())
    {

    }
}

void Settings::onCheckedS()
{
    if(ui->checkS->isChecked())
    {

    }
}

void Settings::onCheckedM()
{
    if(ui->checkM->isChecked())
    {

    }
}

void Settings::onCheckedF()
{
    if(ui->checkF->isChecked())
    {

    }
}

void Settings::onCheckedK()
{
    if(ui->checkK->isChecked())
    {

    }
}

void Settings::onCheckedN()
{
    if(ui->checkN->isChecked())
    {

    }
}

void Settings::onCheckedE()
{
    if(ui->checkE->isChecked())
    {

    }
}

void Settings::onCheckedA()
{
    if(ui->checkA->isChecked())
    {

    }
}

void Settings::onCheckedW()
{
    if(ui->checkW->isChecked())
    {

    }
}

void Settings::onCheckedPort()
{
    if(ui->checkPort->isChecked())
    {

    }
}

void Settings::onCheckedIpId()
{
    if(ui->checkIpId->isChecked())
    {

    }
}

void Settings::onCheckedDnsAddr()
{

}

void Settings::onCheckedDnsPort()
{

}

void Settings::onCheckedDnsv6Addr()
{

}

void Settings::onCheckedDnsv6Port()
{

}

void Settings::onCheckedBlacklist()
{

}
