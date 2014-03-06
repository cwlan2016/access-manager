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

int Mac::vlan() const
{
    return mVlan;
}

void Mac::setVlan(int vlanTag)
{
    mVlan = vlanTag;
}
