#include "settings.h"
#include "ui_settings.h"

#include <QDebug>

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ayarR(new QSettings("HexOpenSource", "GBDPI-GUI")),
    spinDnsPortValue(1253),
    lineDnsAddrValue("208.67.220.220"),
    spinDnsv6PortValue(1253),
    lineDnsv6AddrValue("2a02:6b8::feed:0ff"),
    spinEValue(0),
    spinFValue(0),
    spinKValue(0),
    spinPortValue(0),
    lineIpIdValue(""),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    setWindowIcon(QIcon(":/images/images/settings-gears-button.png"));
    setWindowTitle("Ayarlar");

    //Reference to QSettings for capturing it from lambda function
    auto& ayar = ayarR;

    ui->radioQuick1->setChecked(true);

    //Determine if its first run then create or not registry settings
    //TODO: Inherit from QSettings and make efficient save/restore/load mechanism.
    if(!ayarR->contains("Parametre/defaultParam"))
    {
        resetSettings();
    }

    //SIGNAL&SLOTS for parameters and inputs change
    // TODO: Turn all Checkbox::clicked signals to CheckBox::stateChanged
    connect(ui->checkDefaultParam, &QCheckBox::stateChanged, this, &Settings::onCheckedDefaultParam);
    connect(ui->checkCustomParam, &QCheckBox::clicked, this, &Settings::onCheckedCustomParam);
    connect(ui->checkQuickSettings, &QCheckBox::clicked, this, &Settings::onCheckedQuickSettings);

    connect(ui->spinF, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this, ayar](){
        if(ui->checkF->isChecked()){
            spinFValue = ui->spinF->value();
            ayar->setValue("Parametre/paramF", "-f " + QString::number(spinFValue));
        }
    });
    connect(ui->spinK, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this, ayar](){
        if(ui->checkK->isChecked()){
            spinKValue = ui->spinK->value();
            ayar->setValue("Parametre/paramK", "-k " + QString::number(spinKValue));
        }
    });
    connect(ui->spinE, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this, ayar](){
        if(ui->checkE->isChecked()){
            spinEValue = ui->spinE->value();
            ayar->setValue("Parametre/paramE", "-e " + QString::number(spinEValue));
        }
    });
    connect(ui->spinPort, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this, ayar](){
        if(ui->checkPort->isChecked())
        {
            spinPortValue = ui->spinPort->value();
            ayar->setValue("Parametre/paramPort", "--port " + QString::number(spinPortValue));
        }
    });
    connect(ui->spinDnsPort, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this, ayar](){
        if(ui->checkDnsPort->isChecked())
        {
            spinDnsPortValue = ui->spinDnsPort->value();
            ayar->setValue("Parametre/paramDnsPort", "--dns-port " + QString::number(spinDnsPortValue));
            //ayar->sync();
        }
    });
    connect(ui->spinDnsv6Port, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this, ayar](){
        if(ui->checkDnsv6Port->isChecked())
        {
            spinDnsv6PortValue = ui->spinDnsv6Port->value();
            ayar->setValue("Parametre/paramDnsv6Port", "--dnsv6-port " + QString::number(spinDnsv6PortValue));
            //ayar->sync();
        }
    });

    connect(ui->lineIpId, &QLineEdit::textChanged, [this, ayar](){
        if(ui->checkIpId->isChecked())
        {
            lineIpIdValue = ui->lineIpId->text();
            ayar->setValue("Parametre/paramIpId", "--ip-id " + lineIpIdValue);
            //ayar->sync();
        }
    });
    connect(ui->lineDnsAddr, &QLineEdit::textChanged, [this, ayar](){
        if(ui->checkDnsAddr->isChecked())
        {
            lineDnsAddrValue = ui->lineDnsAddr->text();
            ayar->setValue("Parametre/paramDnsAddr", "--dns-addr " + lineDnsAddrValue);
            //ayar->sync();
        }
    });
    connect(ui->lineDnsv6Addr, &QLineEdit::textChanged, [this, ayar](){
        if(ui->checkDnsv6Addr->isChecked())
        {
            lineDnsv6AddrValue = ui->lineDnsv6Addr->text();
            ayar->setValue("Parametre/paramDnsv6Addr", "--dnsv6-addr " + lineDnsv6AddrValue);
            //ayar->sync();
        }
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
    connect(ui->checkDnsAddr, &QCheckBox::clicked, this, &Settings::onCheckedDnsAddr);
    connect(ui->checkDnsPort, &QCheckBox::clicked, this, &Settings::onCheckedDnsPort);
    connect(ui->checkDnsv6Addr, &QCheckBox::clicked, this, &Settings::onCheckedDnsv6Addr);
    connect(ui->checkDnsv6Port, &QCheckBox::clicked, this, &Settings::onCheckedDnsv6Port);

    connect(ui->radioQuick1, &QRadioButton::clicked, this, &Settings::onCheckedRadioQuick1);
    connect(ui->radioQuick2, &QRadioButton::clicked, this, &Settings::onCheckedRadioQuick2);
    connect(ui->radioQuick3, &QRadioButton::clicked, this, &Settings::onCheckedRadioQuick3);
    connect(ui->radioQuick4, &QRadioButton::clicked, this, &Settings::onCheckedRadioQuick4);

    connect(ui->startupBox, &QCheckBox::clicked, this, &Settings::onCheckedStartup);
    connect(ui->trayBox, &QCheckBox::clicked, this, &Settings::onCheckedSystemTray);

    loadSettings();
}

Settings::~Settings()
{
    delete ui;
}

void Settings::ayarKayit()
{
    ayarR->sync();
}

void Settings::onCheckedDefaultParam()
{
    if(ui->checkDefaultParam->isChecked())
    {
        ui->paramBox->setEnabled(false);
        ui->checkCustomParam->setCheckState(Qt::Unchecked);
        ayarR->setValue("Parametre/defaultParam", true);
        ayarR->setValue("Parametre/customParam", false);
        ayarR->setValue("Parametre/quickSettings", false);
    }
    else
    {

    }
    //Capturing change from MainWindow, Thus we can control ComboBox for Parameter List.
    emit defaultParamStateChanged(ui->checkDefaultParam->checkState());
}

void Settings::onCheckedCustomParam()
{
    if(ui->checkCustomParam->isChecked())
    {
        ui->checkDefaultParam->setChecked(Qt::Unchecked);
        ui->paramBox->setEnabled(true);
        ayarR->setValue("Parametre/customParam", true);
        ayarR->setValue("Parametre/quickSettings", false);
        if(ayarR->value("quickSettings").toBool())  ayarR->setValue("Parametre/quickSettings", true);
        ayarR->setValue("Parametre/defaultParam", false);
    }
    else
    {
        ui->paramBox->setEnabled(false);
    }
}

void Settings::onCheckedSystemTray()
{
    if(ui->trayBox->isChecked())
    {
        ayarR->setValue("System/systemTray", true);
    }
    else
    {
        ayarR->setValue("System/systemTray", false);
    }
}

void Settings::onCheckedStartup()
{
    QSettings startup("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if(ui->startupBox->isChecked())
    {
        QString appPath = QCoreApplication::applicationFilePath();
        appPath.replace("/", "\\");
        startup.setValue("GuiForGoodByeDPI", QString(appPath + " -silent").toStdString().c_str());
        ayarR->setValue("System/systemTray", true);
    }
    else
    {
        startup.remove("GuiForGoodByeDPI");
        ayarR->setValue("System/systemTray", false);
    }
}

void Settings::onCheckedSchedule()
{
    if(ui->scheduleBox->isChecked())
    {
        ayarR->setValue("System/systemSchedule", true);
        ayarR->setValue("System/systemScheduleStart", true);
        ayarR->setValue("System/systemScheduleEnd", true);
        ayarR->setValue("System/systemScheduleDays", true);
    }
    else
    {
        ayarR->setValue("System/systemSchedule", false);
    }
}

void Settings::onCheckedQuickSettings()
{
    if(ui->checkQuickSettings->isChecked())
    {
        ui->groupQuickSettings->setEnabled(true);
        ui->param1->setEnabled(false);
        ayarR->setValue("Parametre/quickSettings", true);
        ayarR->setValue("Parametre/customParam", true);
        ayarR->setValue("Parametre/defaultParam", false);

        emit ui->radioQuick1->clicked(true);
    }
    else
    {
        ui->groupQuickSettings->setEnabled(false);
        ui->param1->setEnabled(true);
        ayarR->setValue("Parametre/paramQuick", false);
    }
}

void Settings::onCheckedP()
{
    if(ui->checkP->isChecked())
    {
        ayarR->setValue("Parametre/paramP", true);
    }
    else
    {
        ayarR->setValue("Parametre/paramP", false);
    }
}

void Settings::onCheckedR()
{
    if(ui->checkP->isChecked())
    {
        ayarR->setValue("Parametre/paramR", true);
    }
    else
    {
        ayarR->setValue("Parametre/paramR", false);
    }
}

void Settings::onCheckedS()
{
    if(ui->checkS->isChecked())
    {
        ayarR->setValue("Parametre/paramS", true);
    }
    else
    {
        ayarR->setValue("Parametre/paramS", false);
    }
}

void Settings::onCheckedM()
{
    if(ui->checkM->isChecked())
    {
        ayarR->setValue("Parametre/paramM", true);
    }
    else
    {
        ayarR->setValue("Parametre/paramM", false);
    }
}

void Settings::onCheckedF()
{
    if(ui->checkF->isChecked())
    {
        ayarR->setValue("Parametre/paramF", "-f " + QString::number(spinFValue));
    }
    else
    {
        ayarR->setValue("Parametre/paramF", false);
    }
}

void Settings::onCheckedK()
{
    if(ui->checkK->isChecked())
    {
        ayarR->setValue("Parametre/paramK", "-k " + QString::number(spinKValue));
    }
    else
    {
        ayarR->setValue("Parametre/paramK", false);
    }
}

void Settings::onCheckedN()
{
    if(ui->checkN->isChecked())
    {
        ayarR->setValue("Parametre/paramN", true);
    }
    else
    {
        ayarR->setValue("Parametre/paramN", false);
    }
}

void Settings::onCheckedE()
{
    if(ui->checkE->isChecked())
    {
        ayarR->setValue("Parametre/paramE", "-e " + QString::number(spinEValue));
    }
    else
    {
        ayarR->setValue("Parametre/paramE", false);
    }
}

void Settings::onCheckedA()
{
    if(ui->checkA->isChecked())
    {
        ayarR->setValue("Parametre/paramA", true);
    }
    else
    {
        ayarR->setValue("Parametre/paramA", false);
    }
}

void Settings::onCheckedW()
{
    if(ui->checkW->isChecked())
    {
        ayarR->setValue("Parametre/paramW", true);
    }
    else
    {
        ayarR->setValue("Parametre/paramW", false);
    }
}

void Settings::onCheckedPort()
{
    if(ui->checkPort->isChecked())
    {
        ayarR->setValue("Parametre/paramPort", "--port " + QString::number(spinPortValue));
    }
    else
    {
        ayarR->setValue("Parametre/paramPort", false);
    }
}

void Settings::onCheckedIpId()
{
    if(ui->checkIpId->isChecked())
    {
        ayarR->setValue("Parametre/paramIpId", "--ip-id " + lineIpIdValue);
    }
    else
    {
        ayarR->setValue("Parametre/paramIpId", false);
    }
}

void Settings::onCheckedDnsAddr()
{
    if(ui->checkDnsAddr->isChecked())
    {
        ayarR->setValue("Parametre/paramDnsAddr", "--dns-addr " + lineDnsAddrValue);
    }
    else
    {
        ayarR->setValue("Parametre/paramDnsAddr", false);
    }
}

void Settings::onCheckedDnsPort()
{
    if(ui->checkDnsPort->isChecked())
    {
        ayarR->setValue("Parametre/paramDnsPort", "--dns-port " + QString::number(spinDnsPortValue));
    }
    else
    {
        ayarR->setValue("Parametre/paramDnsPort", false);
    }
}

void Settings::onCheckedDnsv6Addr()
{
    if(ui->checkDnsv6Addr->isChecked())
    {
        ayarR->setValue("Parametre/paramDnsv6Addr", "--dnsv6-addr " + lineDnsv6AddrValue);
    }
    else
    {
        ayarR->setValue("Parametre/paramDnsv6Addr", false);
    }
}

void Settings::onCheckedDnsv6Port()
{
    if(ui->checkDnsv6Port->isChecked())
    {
        ayarR->setValue("Parametre/paramDnsv6Port", "--dnsv6-port " + QString::number(spinDnsv6PortValue));
    }
    else
    {
        ayarR->setValue("Parametre/paramDnsv6Port", false);
    }
}

void Settings::onCheckedBlacklist()
{
    if(ui->checkBlacklist->isChecked())
    {
        ayarR->setValue("Parametre/paramBlacklist", true);
    }
    else
    {
        ayarR->setValue("Parametre/paramBlacklist", false);
    }
}

void Settings::onCheckedRadioQuick1()
{
    ayarR->setValue("Parametre/paramQuick", "-1");
}

void Settings::onCheckedRadioQuick2()
{
    ayarR->setValue("Parametre/paramQuick", "-2");
}

void Settings::onCheckedRadioQuick3()
{
    ayarR->setValue("Parametre/paramQuick", "-3");
}

void Settings::onCheckedRadioQuick4()
{
    ayarR->setValue("Parametre/paramQuick", "-4");
}

void Settings::resetSettings()
{
    ayarR->clear();

    ayarR->beginGroup("Parametre");
    ayarR->setValue("defaultParam", true);
    ayarR->setValue("customParam", false);
    ayarR->setValue("quickSettings", false);
    ayarR->setValue("paramP", false);
    ayarR->setValue("paramR", false);
    ayarR->setValue("paramS", false);
    ayarR->setValue("paramF", false);
    ayarR->setValue("paramK", false);
    ayarR->setValue("paramN", false);
    ayarR->setValue("paramE", false);
    ayarR->setValue("paramA", false);
    ayarR->setValue("paramW", false);
    ayarR->setValue("paramPort", false);
    ayarR->setValue("paramIpId", false);
    ayarR->setValue("paramDnsAddr", false);
    ayarR->setValue("paramDnsPort", false);
    ayarR->setValue("paramDnsv6Addr", false);
    ayarR->setValue("paramDnsv6Port", false);
    ayarR->setValue("paramBlacklist", false);
    ayarR->setValue("paramQuick", false);
    ayarR->endGroup();

    ayarR->beginGroup("System");
    ayarR->setValue("systemTray", true);
    ayarR->setValue("systemStartup", false);
    ayarR->setValue("systemSchedule", false);
    ayarR->endGroup();
}

/**
 * @brief Settings::loadSettings
 *
 * I aggree with you It's not efficient.
 * It's highest priority for me to make it more efficient.
 * I don't like hard coded things either :)
 *
 */
void Settings::loadSettings()
{
    ayarR->beginGroup("Parametre");
    if(ayarR->value("defaultParam").toBool())
    {
        ui->checkDefaultParam->setChecked(true);
        ui->paramBox->setEnabled(false);
    }
    else
    {
        ui->checkDefaultParam->setChecked(false);
    }

    if(ayarR->value("customParam").toBool())
    {
         ui->checkCustomParam->setChecked(true);
         ui->paramBox->setEnabled(true);
         if(ayarR->value("quickSettings").toBool())
         {
             ui->checkQuickSettings->setEnabled(true);
             ui->param1->setEnabled(false);
         }
    }
    else
    {
        ui->checkCustomParam->setChecked(false);
        ui->paramBox->setEnabled(false);
    }

    if(ayarR->value("quickSettings").toBool())
    {
        ui->checkQuickSettings->setChecked(true);
        ui->groupQuickSettings->setEnabled(true);
        ui->param1->setEnabled(false);
    }
    else
    {
        ui->checkQuickSettings->setChecked(false);
        ui->param1->setEnabled(true);
        ui->groupQuickSettings->setEnabled(false);
    }

    if(ayarR->value("paramP").toBool())
    {
        ui->checkP->setChecked(true);
    }
    else
    {
        ui->checkP->setChecked(false);
    }

    if(ayarR->value("paramR").toBool())
    {
        ui->checkR->setChecked(true);
    }
    else
    {
        ui->checkR->setChecked(false);
    }

    if(ayarR->value("paramS").toBool())
    {
        ui->checkS->setChecked(true);
    }
    else
    {
        ui->checkS->setChecked(false);
    }

    if(ayarR->value("paramF").toString() != "false")
    {
        ui->checkF->setChecked(true);
        QString tmp = ayarR->value("paramF").toString();
        QStringList list = tmp.split(" ");

        ui->spinF->setValue(list.at(1).toInt());
        spinFValue = list.at(1).toInt();
    }
    else
    {
        ui->checkF->setChecked(false);
    }

    if(ayarR->value("paramK").toString() != "false")
    {
        ui->checkK->setChecked(true);
        QString tmp = ayarR->value("paramK").toString();
        QStringList list = tmp.split(" ");

        ui->spinK->setValue(list.at(1).toInt());
        spinKValue = list.at(1).toInt();
    }
    else
    {
        ui->checkF->setChecked(false);
    }

    if(ayarR->value("paramN").toBool())
    {
        ui->checkN->setChecked(true);
    }
    else
    {
        ui->checkN->setChecked(false);
    }

    if(ayarR->value("paramE").toString() != "false")
    {
        ui->checkE->setChecked(true);
        QString tmp = ayarR->value("paramE").toString();
        QStringList list = tmp.split(" ");

        ui->spinE->setValue(list.at(1).toInt());
        spinEValue = list.at(1).toInt();
    }
    else
    {
        ui->checkE->setChecked(false);
    }

    if(ayarR->value("paramA").toBool())
    {
        ui->checkA->setChecked(true);
    }
    else
    {
        ui->checkA->setChecked(false);
    }

    if(ayarR->value("paramW").toBool())
    {
        ui->checkW->setChecked(true);
    }
    else
    {
        ui->checkW->setChecked(false);
    }

    if(ayarR->value("paramPort").toString() != "false")
    {
        ui->checkPort->setChecked(true);
        QString tmp = ayarR->value("paramPort").toString();
        QStringList list = tmp.split(" ");

        ui->spinPort->setValue(list.at(1).toInt());
        spinPortValue = list.at(1).toInt();
    }
    else
    {
        ui->checkPort->setChecked(false);
    }

    if(ayarR->value("paramIpId").toString() != "false")
    {
        ui->checkIpId->setChecked(true);
        QString tmp = ayarR->value("paramIpId").toString();
        QStringList list = tmp.split(" ");

        ui->lineIpId->setText(list.at(1));
        lineIpIdValue = list.at(1);
    }
    else
    {
        ui->checkIpId->setChecked(false);
    }

    if(ayarR->value("paramDnsAddr").toString() != "false")
    {
        ui->checkDnsAddr->setChecked(true);
        QString tmp = ayarR->value("paramDnsAddr").toString();
        QStringList list = tmp.split(" ");

        ui->lineDnsAddr->setText(list.at(1));
        lineDnsAddrValue = list.at(1);
    }
    else
    {
        ui->checkDnsAddr->setChecked(false);
    }

    if(ayarR->value("paramDnsPort").toString() != "false")
    {
        ui->checkDnsPort->setChecked(true);
        QString tmp = ayarR->value("paramDnsPort").toString();
        QStringList list = tmp.split(" ");

        ui->spinDnsPort->setValue(list.at(1).toInt());
        spinDnsPortValue = list.at(1).toInt();
    }
    else
    {
        ui->checkDnsPort->setChecked(false);
    }

    if(ayarR->value("paramDnsv6Addr").toString() != "false")
    {
        ui->checkDnsv6Addr->setChecked(true);
        QString tmp = ayarR->value("paramDnsv6Addr").toString();
        QStringList list = tmp.split(" ");

        ui->lineDnsv6Addr->setText(list.at(1));
        lineDnsv6AddrValue = list.at(1);
    }
    else
    {
        ui->checkDnsv6Addr->setChecked(false);
    }

    if(ayarR->value("paramDnsv6Port").toString() != "false")
    {
        ui->checkDnsv6Port->setChecked(true);
        QString tmp = ayarR->value("paramDnsv6Port").toString();
        QStringList list = tmp.split(" ");

        ui->spinDnsv6Port->setValue(list.at(1).toInt());
        spinDnsv6PortValue = list.at(1).toInt();
    }
    else
    {
        ui->checkDnsv6Port->setChecked(false);
    }

    if(ayarR->value("paramBlacklist").toBool())
    {
        ui->checkBlacklist->setChecked(true);
    }
    else
    {
        ui->checkBlacklist->setChecked(false);
    }

    ayarR->endGroup();
}
