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
    // TODO: Turn all Checkbox::stateChanged signals to CheckBox::stateChanged
    connect(ui->checkDefaultParam, &QCheckBox::stateChanged, this, &Settings::onCheckedDefaultParam);
    connect(ui->checkCustomParam, &QCheckBox::stateChanged, this, &Settings::onCheckedCustomParam);
    connect(ui->checkQuickSettings, &QCheckBox::stateChanged, this, &Settings::onCheckedQuickSettings);

    connect(ui->spinF, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this, ayar](){
        if(ui->checkF->checkState() == Qt::Checked){
            spinFValue = ui->spinF->value();
            ayar->setValue("Parametre/paramF", "-f " + QString::number(spinFValue));
        }
    });
    connect(ui->spinK, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this, ayar](){
        if(ui->checkK->checkState() == Qt::Checked){
            spinKValue = ui->spinK->value();
            ayar->setValue("Parametre/paramK", "-k " + QString::number(spinKValue));
        }
    });
    connect(ui->spinE, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this, ayar](){
        if(ui->checkE->checkState() == Qt::Checked){
            spinEValue = ui->spinE->value();
            ayar->setValue("Parametre/paramE", "-e " + QString::number(spinEValue));
        }
    });
    connect(ui->spinPort, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this, ayar](){
        if(ui->checkPort->checkState() == Qt::Checked)
        {
            spinPortValue = ui->spinPort->value();
            ayar->setValue("Parametre/paramPort", "--port " + QString::number(spinPortValue));
        }
    });
    connect(ui->spinDnsPort, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this, ayar](){
        if(ui->checkDnsPort->checkState() == Qt::Checked)
        {
            spinDnsPortValue = ui->spinDnsPort->value();
            ayar->setValue("Parametre/paramDnsPort", "--dns-port " + QString::number(spinDnsPortValue));
        }
    });
    connect(ui->spinDnsv6Port, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this, ayar](){
        if(ui->checkDnsv6Port->checkState() == Qt::Checked)
        {
            spinDnsv6PortValue = ui->spinDnsv6Port->value();
            ayar->setValue("Parametre/paramDnsv6Port", "--dnsv6-port " + QString::number(spinDnsv6PortValue));
        }
    });

    connect(ui->lineIpId, &QLineEdit::textChanged, [this, ayar](){
        if(ui->checkIpId->checkState() == Qt::Checked)
        {
            lineIpIdValue = ui->lineIpId->text();
            ayar->setValue("Parametre/paramIpId", "--ip-id " + lineIpIdValue);
        }
    });
    connect(ui->lineDnsAddr, &QLineEdit::textChanged, [this, ayar](){
        if(ui->checkDnsAddr->checkState() == Qt::Checked)
        {
            lineDnsAddrValue = ui->lineDnsAddr->text();
            ayar->setValue("Parametre/paramDnsAddr", "--dns-addr " + lineDnsAddrValue);
        }
    });
    connect(ui->lineDnsv6Addr, &QLineEdit::textChanged, [this, ayar](){
        if(ui->checkDnsv6Addr->checkState() == Qt::Checked)
        {
            lineDnsv6AddrValue = ui->lineDnsv6Addr->text();
            qDebug() << "1: " + lineDnsv6AddrValue;
            ayar->setValue("Parametre/paramDnsv6Addr", "--dnsv6-addr " + lineDnsv6AddrValue);
        }
    });

    connect(ui->checkP, &QCheckBox::stateChanged, this, &Settings::onCheckedP);
    connect(ui->checkR, &QCheckBox::stateChanged, this, &Settings::onCheckedR);
    connect(ui->checkS, &QCheckBox::stateChanged, this, &Settings::onCheckedS);
    connect(ui->checkM, &QCheckBox::stateChanged, this, &Settings::onCheckedM);
    connect(ui->checkF, &QCheckBox::stateChanged, this, &Settings::onCheckedF);
    connect(ui->checkK, &QCheckBox::stateChanged, this, &Settings::onCheckedK);
    connect(ui->checkN, &QCheckBox::stateChanged, this, &Settings::onCheckedN);
    connect(ui->checkE, &QCheckBox::stateChanged, this, &Settings::onCheckedE);
    connect(ui->checkA, &QCheckBox::stateChanged, this, &Settings::onCheckedA);
    connect(ui->checkW, &QCheckBox::stateChanged, this, &Settings::onCheckedW);
    connect(ui->checkPort, &QCheckBox::stateChanged, this, &Settings::onCheckedPort);
    connect(ui->checkIpId, &QCheckBox::stateChanged, this, &Settings::onCheckedIpId);
    connect(ui->checkDnsAddr, &QCheckBox::stateChanged, this, &Settings::onCheckedDnsAddr);
    connect(ui->checkDnsPort, &QCheckBox::stateChanged, this, &Settings::onCheckedDnsPort);
    connect(ui->checkDnsv6Addr, &QCheckBox::stateChanged, this, &Settings::onCheckedDnsv6Addr);
    connect(ui->checkDnsv6Port, &QCheckBox::stateChanged, this, &Settings::onCheckedDnsv6Port);

    connect(ui->radioQuick1, &QRadioButton::clicked, this, &Settings::onCheckedRadioQuick1);
    connect(ui->radioQuick2, &QRadioButton::clicked, this, &Settings::onCheckedRadioQuick2);
    connect(ui->radioQuick3, &QRadioButton::clicked, this, &Settings::onCheckedRadioQuick3);
    connect(ui->radioQuick4, &QRadioButton::clicked, this, &Settings::onCheckedRadioQuick4);

    connect(ui->startupBox, &QCheckBox::stateChanged, this, &Settings::onCheckedStartup);
    connect(ui->trayBox, &QCheckBox::stateChanged, this, &Settings::onCheckedSystemTray);
    connect(ui->notificationBox, &QCheckBox::stateChanged, this, &Settings::onCheckedNotification);

    loadSettings();
}

Settings::~Settings()
{
    delete ui;
}

void Settings::closeEvent(QCloseEvent *event)
{
    if(this->isVisible() || this->isTopLevel())
    {
        event->ignore();
        this->hide();
    }
    else
    {
    }

    emit defaultParamStateChanged(ui->checkDefaultParam->checkState());
}

void Settings::onCheckedDefaultParam()
{
    if(ui->checkDefaultParam->checkState() == Qt::Checked)
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
    if(ui->checkCustomParam->checkState() == Qt::Checked)
    {
        ui->checkDefaultParam->setChecked(Qt::Unchecked);
        ui->paramBox->setEnabled(true);
        ayarR->setValue("Parametre/customParam", true);
        if(ayarR->value("Parametre/quickSettings").toBool()){
            ui->groupQuickSettings->setEnabled(true);
        }
        else
        {
            ui->groupQuickSettings->setEnabled(false);
        }
        ayarR->setValue("Parametre/defaultParam", false);
    }
    else
    {
        ui->paramBox->setEnabled(false);
        ayarR->setValue("Parametre/customParam", false);
    }
    emit defaultParamStateChanged(ui->checkDefaultParam->checkState());
}

void Settings::onCheckedSystemTray()
{
    if(ui->trayBox->checkState() == Qt::Checked)
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
    if(ui->startupBox->checkState() == Qt::Checked)
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
    if(ui->scheduleBox->checkState() == Qt::Checked)
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

void Settings::onCheckedNotification()
{
    if(ui->notificationBox->checkState() == Qt::Checked)
    {
        ayarR->setValue("System/disableNotifications", true);
    }
    else
    {
        ayarR->setValue("System/disableNotifications", false);
    }
}

void Settings::onCheckedQuickSettings()
{
    if(ui->checkQuickSettings->checkState() == Qt::Checked)
    {
        ui->groupQuickSettings->setEnabled(true);
        ui->param1->setEnabled(false);
        ayarR->setValue("Parametre/quickSettings", true);
        ayarR->setValue("Parametre/customParam", true);
        ayarR->setValue("Parametre/defaultParam", false);

        if(ayarR->value("Parametre/paramQuick").toString() == "-1")
            ui->radioQuick1->setChecked(true);
        if(ayarR->value("Parametre/paramQuick").toString() == "-2")
            ui->radioQuick2->setChecked(true);
        if(ayarR->value("Parametre/paramQuick").toString() == "-3")
            ui->radioQuick3->setChecked(true);
        if(ayarR->value("Parametre/paramQuick").toString() == "-4")
            ui->radioQuick4->setChecked(true);
    }
    else
    {
        ui->groupQuickSettings->setEnabled(false);
        ui->param1->setEnabled(true);
        ayarR->setValue("Parametre/quickSettings", false);
    }
    emit defaultParamStateChanged(ui->checkDefaultParam->checkState());
}

void Settings::onCheckedP()
{
    if(ui->checkP->checkState() == Qt::Checked)
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
    if(ui->checkR->checkState() == Qt::Checked)
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
    if(ui->checkS->checkState() == Qt::Checked)
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
    if(ui->checkM->checkState() == Qt::Checked)
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
    if(ui->checkF->checkState() == Qt::Checked)
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
    if(ui->checkK->checkState() == Qt::Checked)
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
    if(ui->checkN->checkState() == Qt::Checked)
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
    if(ui->checkE->checkState() == Qt::Checked)
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
    if(ui->checkA->checkState() == Qt::Checked)
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
    if(ui->checkW->checkState() == Qt::Checked)
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
    if(ui->checkPort->checkState() == Qt::Checked)
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
    if(ui->checkIpId->checkState() == Qt::Checked)
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
    if(ui->checkDnsAddr->checkState() == Qt::Checked)
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
    if(ui->checkDnsPort->checkState() == Qt::Checked)
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
    if(ui->checkDnsv6Addr->checkState() == Qt::Checked)
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
    if(ui->checkDnsv6Port->checkState() == Qt::Checked)
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
    if(ui->checkBlacklist->checkState() == Qt::Checked)
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
    ayarR->setValue("disableNotifications", false);
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
    //ayarR->beginGroup("Parametre");
    if(ayarR->value("Parametre/defaultParam").toBool())
    {
        ui->checkDefaultParam->setChecked(true);
        ui->paramBox->setEnabled(false);
    }
    else
    {
        ui->checkDefaultParam->setChecked(false);
    }

    if(ayarR->value("Parametre/customParam").toBool())
    {
         ui->checkCustomParam->setChecked(true);
         ui->paramBox->setEnabled(true);
         if(ayarR->value("Parametre/quickSettings").toBool())
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

    if(ayarR->value("Parametre/quickSettings").toBool())
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

    if(ayarR->value("Parametre/paramQuick").toString() != "false")
    {
        if(ayarR->value("Parametre/paramQuick").toString() == "-1")
            ui->radioQuick1->setChecked(true);
        else if(ayarR->value("Parametre/paramQuick").toString() == "-2")
            ui->radioQuick2->setChecked(true);
        else if(ayarR->value("Parametre/paramQuick").toString() == "-3")
            ui->radioQuick3->setChecked(true);
        else if(ayarR->value("Parametre/paramQuick").toString() == "-4")
            ui->radioQuick4->setChecked(true);

    }

    if(ayarR->value("Parametre/paramP").toBool())
    {
        ui->checkP->setChecked(true);
    }
    else
    {
        ui->checkP->setChecked(false);
    }

    if(ayarR->value("Parametre/paramR").toBool())
    {
        ui->checkR->setChecked(true);
    }
    else
    {
        ui->checkR->setChecked(false);
    }

    if(ayarR->value("Parametre/paramS").toBool())
    {
        ui->checkS->setChecked(true);
    }
    else
    {
        ui->checkS->setChecked(false);
    }

    if(ayarR->value("Parametre/paramM").toBool())
    {
        ui->checkM->setChecked(true);
    }
    else
    {
        ui->checkM->setChecked(false);
    }

    if(ayarR->value("Parametre/paramF").toString() != "false")
    {
        ui->checkF->setChecked(true);
        QString tmp = ayarR->value("Parametre/paramF").toString();
        QStringList list = tmp.split(" ");

        ui->spinF->setValue(list.at(1).toInt());
        spinFValue = list.at(1).toInt();
    }
    else
    {
        ui->checkF->setChecked(false);
    }

    if(ayarR->value("Parametre/paramK").toString() != "false")
    {
        ui->checkK->setChecked(true);
        QString tmp = ayarR->value("Parametre/paramK").toString();
        QStringList list = tmp.split(" ");

        ui->spinK->setValue(list.at(1).toInt());
        spinKValue = list.at(1).toInt();
    }
    else
    {
        ui->checkF->setChecked(false);
    }

    if(ayarR->value("Parametre/paramN").toBool())
    {
        ui->checkN->setChecked(true);
    }
    else
    {
        ui->checkN->setChecked(false);
    }

    if(ayarR->value("Parametre/paramE").toString() != "false")
    {
        ui->checkE->setChecked(true);
        QString tmp = ayarR->value("Parametre/paramE").toString();
        QStringList list = tmp.split(" ");

        ui->spinE->setValue(list.at(1).toInt());
        spinEValue = list.at(1).toInt();
    }
    else
    {
        ui->checkE->setChecked(false);
    }

    if(ayarR->value("Parametre/paramA").toBool())
    {
        ui->checkA->setChecked(true);
    }
    else
    {
        ui->checkA->setChecked(false);
    }

    if(ayarR->value("Parametre/paramW").toBool())
    {
        ui->checkW->setChecked(true);
    }
    else
    {
        ui->checkW->setChecked(false);
    }

    if(ayarR->value("Parametre/paramPort").toString() != "false")
    {
        ui->checkPort->setChecked(true);
        QString tmp = ayarR->value("Parametre/paramPort").toString();
        QStringList list = tmp.split(" ");

        ui->spinPort->setValue(list.at(1).toInt());
        spinPortValue = list.at(1).toInt();
    }
    else
    {
        ui->checkPort->setChecked(false);
    }

    if(ayarR->value("Parametre/paramIpId").toString() != "false")
    {
        ui->checkIpId->setChecked(true);
        QString tmp = ayarR->value("Parametre/paramIpId").toString();
        QStringList list = tmp.split(" ");

        ui->lineIpId->setText(list.at(1));
        lineIpIdValue = list.at(1);
    }
    else
    {
        ui->checkIpId->setChecked(false);
    }

    if(ayarR->value("Parametre/paramDnsAddr").toString() != "false")
    {
        ui->checkDnsAddr->setChecked(true);
        QString tmp = ayarR->value("Parametre/paramDnsAddr").toString();
        QStringList list = tmp.split(" ");

        ui->lineDnsAddr->setText(list.at(1));
        lineDnsAddrValue = list.at(1);
    }
    else
    {
        ui->checkDnsAddr->setChecked(false);
    }

    if(ayarR->value("Parametre/paramDnsPort").toString() != "false")
    {
        ui->checkDnsPort->setChecked(true);
        QString tmp = ayarR->value("Parametre/paramDnsPort").toString();
        QStringList list = tmp.split(" ");

        ui->spinDnsPort->setValue(list.at(1).toInt());
        spinDnsPortValue = list.at(1).toInt();
    }
    else
    {
        ui->checkDnsPort->setChecked(false);
    }

    if(ayarR->value("Parametre/paramDnsv6Addr").toString() != "false")
    {
        ui->checkDnsv6Addr->setChecked(true);
        QString tmp = ayarR->value("Parametre/paramDnsv6Addr").toString();
        QStringList list = tmp.split(" ");

        ui->lineDnsv6Addr->setText(list.at(1));
        lineDnsv6AddrValue = list.at(1);
        qDebug() << lineDnsv6AddrValue;
    }
    else
    {
        ui->checkDnsv6Addr->setChecked(false);
    }

    if(ayarR->value("Parametre/paramDnsv6Port").toString() != "false")
    {
        ui->checkDnsv6Port->setChecked(true);
        QString tmp = ayarR->value("Parametre/paramDnsv6Port").toString();
        QStringList list = tmp.split(" ");

        ui->spinDnsv6Port->setValue(list.at(1).toInt());
        spinDnsv6PortValue = list.at(1).toInt();
    }
    else
    {
        ui->checkDnsv6Port->setChecked(false);
    }

    if(ayarR->value("Parametre/paramBlacklist").toBool())
    {
        ui->checkBlacklist->setChecked(true);
    }
    else
    {
        ui->checkBlacklist->setChecked(false);
    }

    if(ayarR->value("System/systemStartup").toBool())
    {
        ui->startupBox->setChecked(true);
    }
    else
    {
        ui->startupBox->setChecked(false);
    }

    if(ayarR->value("System/systemTray").toBool())
    {
        ui->trayBox->setChecked(true);
    }
    else
    {
        ui->trayBox->setChecked(false);
    }

    if(ayarR->value("System/disableNotifications").toBool())
    {
        ui->notificationBox->setChecked(false);
    }
    else
    {
        ui->notificationBox->setChecked(true);
    }
}
