#include "macinfo.h"

MacInfo::MacInfo(QObject *parent) :
    QObject(parent)
{
}

int MacInfo::numberPort() const
{
    return mNumberPort;
}

QString MacInfo::mac() const
{
    return mMac;
}

QString MacInfo::vlanName() const
{
    return mVlanName;
}

void MacInfo::setNumberPort(int number)
{
    mNumberPort = number;
}

void MacInfo::setMac(QString mac)
{
    mMac = mac;
}

void MacInfo::setVlanName(QString vlanName)
{
    mVlanName = vlanName;
}
