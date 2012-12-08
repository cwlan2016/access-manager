#include "ntpinfo.h"

NtpInfo::NtpInfo()
{
}

QString NtpInfo::pppLogin()
{
    return mPppLogin;
}

QString NtpInfo::pppPassword()
{
    return mPppPassword;
}

bool NtpInfo::phone1Enable()
{
    return mPhone1Enable;
}

QString NtpInfo::phone1Login()
{
    return mPhone1Login;
}

QString NtpInfo::phone1Password()
{
    return mPhone1Password;
}

bool NtpInfo::phone2Enable()
{
    return mPhone2Enable;
}

QString NtpInfo::phone2Login()
{
    return mPhone2Login;
}

QString NtpInfo::phone2Password()
{
    return mPhone2Password;
}

QString NtpInfo::sipProxy()
{
    return mSipProxy;
}

void NtpInfo::setPppLogin(QString login)
{
    mPppLogin = login;
}

void NtpInfo::setPppPassword(QString password)
{
    mPppPassword = password;
}

void NtpInfo::setPhone1Enable(bool enabled)
{
    mPhone1Enable = enabled;
}

void NtpInfo::setPhone1Login(QString login)
{
    mPhone1Login = login;
}

void NtpInfo::setPhone1Password(QString password)
{
    mPhone1Password = password;
}

void NtpInfo::setPhone2Enable(bool enabled)
{
    mPhone2Enable = enabled;
}

void NtpInfo::setPhone2Login(QString login)
{
    mPhone2Login = login;
}

void NtpInfo::setPhone2Password(QString password)
{
    mPhone2Password = password;
}

void NtpInfo::setSipProxy(QString proxy)
{
    mSipProxy = proxy;
}
