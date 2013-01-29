#ifndef NTPINFO_H
#define NTPINFO_H

#include "ontinfo.h"

class NtpInfo : public OntInfo
{
public:
    NtpInfo();
    QString pppLogin();
    QString pppPassword();
    bool    phone1Enable();
    QString phone1Login();
    QString phone1Password();
    bool    phone2Enable();
    QString phone2Login();
    QString phone2Password();
    QString sipProxy();
    void setPppLogin(QString login);
    void setPppPassword(QString password);
    void setPhone1Enable(bool enabled);
    void setPhone1Login(QString login);
    void setPhone1Password(QString password);
    void setPhone2Enable(bool enabled);
    void setPhone2Login(QString login);
    void setPhone2Password(QString password);
    void setSipProxy(QString proxy);
    void Reconfigure();

    typedef std::shared_ptr<NtpInfo> Ptr;
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

#endif // NTPINFO_H
