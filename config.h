#ifndef CONFIG_H
#define CONFIG_H

#include "Info/snmpconfiginfo.h"

class Config
{
public:
    static bool exist();
    static void toDefault();
    static bool load();
    static bool save();
    static bool backup();
    static QString errorString();
    static void init();
    static QString path();
private:
    static void createSnmpGroup(QSettings &settings);
    static void parseSnmpGroup(QSettings &settings);
    static QString mConfigPath;
    static QString mError;
};

#endif // CONFIG_H
