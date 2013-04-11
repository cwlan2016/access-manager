#ifndef SWITCHCONFIG_H
#define SWITCHCONFIG_H

#ifdef _MSC_VER
#include "../stdafx.h"
#else
#include "stdafx.h"
#endif

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
