#include "mac.h"

Mac::Mac(QObject *parent) :
    QObject(parent)
{
}

int Mac::port() const
{
    return mPort;
}

void Mac::setPort(int number)
{
    mPort = number;
}

QString Mac::mac() const
{
    return mMac;
}

void Mac::setMac(QString mac)
{
    mMac = mac;
}

QString Mac::vlanName() const
{
    return mVlanName;
}

void Mac::setVlanName(QString vlanName)
{
    mVlanName = vlanName;
}
