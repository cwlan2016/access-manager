#include "switchconfiginfo.h"

QString SwitchConfig::inetVlanName()
{
    return mInetVlanName;
}

void SwitchConfig::setInetVlanName(QString vlanName)
{
    mInetVlanName = vlanName;
}

QString SwitchConfig::iptvVlanName()
{
    return mIptvUnicastVlanName;
}

void SwitchConfig::setIptvVlanName(QString vlanName)
{
    mIptvUnicastVlanName = vlanName;
}

void SwitchConfig::toDefault()
{
    mInetVlanName = "Inet";
    mIptvUnicastVlanName = "IPTV_Unicast";
}

QString SwitchConfig::mInetVlanName = QString();
QString SwitchConfig::mIptvUnicastVlanName = QString();
