#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

public slots:
    void onCheckedDefaultParam();
    void onCheckedCustomParam();
    void onCheckedSystemTray();
    void onCheckedStartup();
    void onCheckedSchedule();

    void onCheckedQuickSettings();

    void onCheckedP();
    void onCheckedR();
    void onCheckedS();
    void onCheckedM();
    void onCheckedF();
    void onCheckedK();
    void onCheckedN();
    void onCheckedE();
    void onCheckedA();
    void onCheckedW();
    void onCheckedPort();
    void onCheckedIpId();
    void onCheckedDnsAddr();
    void onCheckedDnsPort();
    void onCheckedDnsv6Addr();
    void onCheckedDnsv6Port();
    void onCheckedBlacklist();


private:
    Ui::Settings *ui;

    int spinFValue;
    int spinKValue;
    int spinEValue;
    int spinPortValue;
    QString lineIpIdValue;

    QString lineDnsAddrValue;
    int spinDnsPortValue;
    QString lineDnsv6AddrValue;
    int spinDnsv6PortValue;
};

#endif // SETTINGS_H
