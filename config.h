#ifndef CONFIG_H
#define CONFIG_H

#include "configs/snmpconfig.h"
#include "configs/switchconfig.h"
#include "configs/dslamprofileconfig.h"

class Config
{
public:
    static void init();

    static bool load();
    static bool save();

    static bool exist();
    static bool backup();

    static void toDefault();

    static QString path();
    static QString error();

private:
    static void createSnmpGroup(QSettings &settings);
    static void parseSnmpGroup(QSettings &settings);

    static void createSwitchGroup(QSettings &settings);
    static void parseSwitchGroup(QSettings &settings);

    static void createDslProfileGroup(QSettings &settings);
    static void parseDslProfileGroup(QSettings &settings);

    static QString mError;
    static QString mConfigPath;
};

#endif // CONFIG_H
