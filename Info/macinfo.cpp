#include "macinfo.h"

MacInfo::MacInfo(QObject *parent) :
    QObject(parent)
{
}

int MacInfo::port() const
{
    return mPort;
}

void MacInfo::setPort(int number)
{
    mPort = number;
}

QString MacInfo::mac() const
{
    return mMac;
}

void MacInfo::setMac(QString mac)
{
    mMac = mac;
}

QString MacInfo::vlanName() const
{
    return mVlanName;
}

void MacInfo::setVlanName(QString vlanName)
{
    mVlanName = vlanName;
}
