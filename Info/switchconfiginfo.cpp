#include "switchconfiginfo.h"

QString SwitchConfigInfo::inetVlanName()
{
    return mInetVlanName;
}

void SwitchConfigInfo::setInetVlanName(QString vlanName)
{
    mInetVlanName = vlanName;
}

QString SwitchConfigInfo::iptvVlanName()
{
    return mIptvUnicastVlanName;
}

void SwitchConfigInfo::setIptvVlanName(QString vlanName)
{
    mIptvUnicastVlanName = vlanName;
}

void SwitchConfigInfo::toDefault()
{
    mInetVlanName = "Inet";
    mIptvUnicastVlanName = "IPTV_Unicast";
}

QString SwitchConfigInfo::mInetVlanName = QString();
QString SwitchConfigInfo::mIptvUnicastVlanName = QString();
