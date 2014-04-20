#include "ntp.h"

Ntp::Ntp(QObject *parent) :
    Ont(parent),
    mPhone1Enable(false),
    mPhone2Enable(false)
{
}

QString Ntp::pppLogin()
{
    return mPppLogin;
}

void Ntp::setPppLogin(QString login)
{
    mPppLogin = login;
}

QString Ntp::pppPassword()
{
    return mPppPassword;
}

void Ntp::setPppPassword(QString password)
{
    mPppPassword = password;
}

bool Ntp::phone1Enable()
{
    return mPhone1Enable;
}

void Ntp::setPhone1Enable(bool enabled)
{
    mPhone1Enable = enabled;
}

QString Ntp::phone1Login()
{
    return mPhone1Login;
}

void Ntp::setPhone1Login(QString login)
{
    mPhone1Login = login;
}

QString Ntp::phone1Password()
{
    return mPhone1Password;
}

void Ntp::setPhone1Password(QString password)
{
    mPhone1Password = password;
}

bool Ntp::phone2Enable()
{
    return mPhone2Enable;
}

void Ntp::setPhone2Enable(bool enabled)
{
    mPhone2Enable = enabled;
}

QString Ntp::phone2Login()
{
    return mPhone2Login;
}

void Ntp::setPhone2Login(QString login)
{
    mPhone2Login = login;
}

QString Ntp::phone2Password()
{
    return mPhone2Password;
}

void Ntp::setPhone2Password(QString password)
{
    mPhone2Password = password;
}

QString Ntp::sipProxy()
{
    return mSipProxy;
}

void Ntp::setSipProxy(QString proxy)
{
    mSipProxy = proxy;
}

void Ntp::Reconfigure()
{
}
