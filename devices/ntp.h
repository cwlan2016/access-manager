#ifndef NTP_H
#define NTP_H

#include "ont.h"

class Ntp : public Ont
{
    Q_OBJECT
public:
    Ntp(QObject *parent);

    QString pppLogin();
    void setPppLogin(QString login);

    QString pppPassword();
    void setPppPassword(QString password);

    bool phone1Enable();
    void setPhone1Enable(bool enabled);

    QString phone1Login();
    void setPhone1Login(QString login);

    QString phone1Password();
    void setPhone1Password(QString password);

    bool phone2Enable();
    void setPhone2Enable(bool enabled);

    QString phone2Login();
    void setPhone2Login(QString login);

    QString phone2Password();
    void setPhone2Password(QString password);

    QString sipProxy();
    void setSipProxy(QString proxy);

    void Reconfigure();

    typedef Ntp *Ptr;

private:
    QString mPppLogin;
    QString mPppPassword;
    bool    mPhone1Enable;
    QString mPhone1Login;
    QString mPhone1Password;
    bool    mPhone2Enable;
    QString mPhone2Login;
    QString mPhone2Password;
    QString mSipProxy;
};

#endif // NTP_H
