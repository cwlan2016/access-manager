#include "ntpinfo.h"

NtpInfo::NtpInfo(QObject *parent) :
    OntInfo(parent)
{
}

QString NtpInfo::pppLogin()
{
    return mPppLogin;
}

void NtpInfo::setPppLogin(QString login)
{
    mPppLogin = login;
}

QString NtpInfo::pppPassword()
{
    return mPppPassword;
}

void NtpInfo::setPppPassword(QString password)
{
    mPppPassword = password;
}

bool NtpInfo::phone1Enable()
{
    return mPhone1Enable;
}

void NtpInfo::setPhone1Enable(bool enabled)
{
    mPhone1Enable = enabled;
}

QString NtpInfo::phone1Login()
{
    return mPhone1Login;
}

void NtpInfo::setPhone1Login(QString login)
{
    mPhone1Login = login;
}

QString NtpInfo::phone1Password()
{
    return mPhone1Password;
}

void NtpInfo::setPhone1Password(QString password)
{
    mPhone1Password = password;
}

bool NtpInfo::phone2Enable()
{
    return mPhone2Enable;
}

void NtpInfo::setPhone2Enable(bool enabled)
{
    mPhone2Enable = enabled;
}

QString NtpInfo::phone2Login()
{
    return mPhone2Login;
}

void NtpInfo::setPhone2Login(QString login)
{
    mPhone2Login = login;
}

QString NtpInfo::phone2Password()
{
    return mPhone2Password;
}

void NtpInfo::setPhone2Password(QString password)
{
    mPhone2Password = password;
}

QString NtpInfo::sipProxy()
{
    return mSipProxy;
}

void NtpInfo::setSipProxy(QString proxy)
{
    mSipProxy = proxy;
}

void NtpInfo::Reconfigure()
{
}
