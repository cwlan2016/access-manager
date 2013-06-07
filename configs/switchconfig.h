#ifndef SWITCHCONFIG_H
#define SWITCHCONFIG_H

#include <stdafx.h>

class SwitchConfig
{
public:
    static QString inetVlanName();
    static void setInetVlanName(QString vlanName);

    static QString iptvVlanName();
    static void setIptvVlanName(QString vlanName);

    static void toDefault();

private:
    static QString mInetVlanName;
    static QString mIptvUnicastVlanName;
};

#endif // SWITCHCONFIG_H
