#include "config.h"

#include "constant.h"

void Config::init()
{
#ifdef Q_OS_WIN
    mConfigPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
                  % "\\Application Data\\" % qApp->organizationName() % "\\";
#else
    mConfigPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
                  % "/." % qApp->organizationName() % "/";
#endif

    DslamProfileConfig::init();
}

bool Config::load()
{
    if (!exist()) {
        toDefault();

        return true;
    }

    QSettings settings(mConfigPath % qApp->applicationName() % ".ini",
                       QSettings::IniFormat);
    parseSnmpGroup(settings);
    parseSwitchGroup(settings);

    return true;
}

bool Config::save()
{
    if (exist())
        backup();

    QSettings settings(mConfigPath % qApp->applicationName() % ".ini",
                       QSettings::IniFormat);
    createSnmpGroup(settings);
    createSwitchGroup(settings);
    settings.sync();

    return true;
}

bool Config::exist()
{
    QFileInfo fileInfo(mConfigPath % qApp->applicationName() % ".ini");

    if (!fileInfo.dir().exists())
        fileInfo.dir().mkpath(fileInfo.dir().path());

    return fileInfo.exists();
}

bool Config::backup()
{
    if (!exist())
        return false;

    QFile::remove(mConfigPath % qApp->applicationName() % ".bak");
    QFile::copy(mConfigPath % qApp->applicationName() % ".ini",
                mConfigPath % qApp->applicationName() % ".bak");

    return true;
}

void Config::toDefault()
{
    SnmpConfig::toDefault();
    SwitchConfig::toDefault();
    QSettings settings(mConfigPath % qApp->applicationName() % ".ini",
                       QSettings::IniFormat);
    createSnmpGroup(settings);
    createSwitchGroup(settings);
    settings.sync();
}

QString Config::path()
{
    return mConfigPath;
}

QString Config::error()
{
    return mError;
}

void Config::createSnmpGroup(QSettings &settings)
{
    settings.beginGroup("snmp");

    settings.setValue(SnmpSettingsStrings::port, SnmpConfig::port());
    settings.setValue(SnmpSettingsStrings::readCommunity,
                      SnmpConfig::readCommunity());
    settings.setValue(SnmpSettingsStrings::writeCommunity,
                      SnmpConfig::writeCommunity());
    settings.setValue(SnmpSettingsStrings::retries, SnmpConfig::retries());
    settings.setValue(SnmpSettingsStrings::timeout, SnmpConfig::timeout());
    settings.setValue(SnmpSettingsStrings::saveConfigTimeout,
                      SnmpConfig::saveConfigTimeout());

    settings.endGroup();
}

void Config::parseSnmpGroup(QSettings &settings)
{
    settings.beginGroup("snmp");

    QVariant value = settings.value(SnmpSettingsStrings::port);
    SnmpConfig::setPort(value.toInt());
    value = settings.value(SnmpSettingsStrings::readCommunity);
    SnmpConfig::setReadCommunity(value.toString());
    value = settings.value(SnmpSettingsStrings::writeCommunity);
    SnmpConfig::setWriteCommunity(value.toString());
    value = settings.value(SnmpSettingsStrings::retries);
    SnmpConfig::setRetries(value.toInt());
    value = settings.value(SnmpSettingsStrings::timeout);
    SnmpConfig::setTimeout(value.toInt());
    value = settings.value(SnmpSettingsStrings::saveConfigTimeout);
    SnmpConfig::setSaveConfigTimeout(value.toInt());

    settings.endGroup();
}

void Config::createSwitchGroup(QSettings &settings)
{
    settings.beginGroup("switch");

    settings.setValue(SwitchSettingsStrings::inetVlanName,
                      SwitchConfig::inetVlanName());
    settings.setValue(SwitchSettingsStrings::iptvUnicastVlanName,
                      SwitchConfig::iptvVlanName());

    settings.endGroup();
}

void Config::parseSwitchGroup(QSettings &settings)
{
    settings.beginGroup("switch");

    QVariant value = settings.value(SwitchSettingsStrings::inetVlanName);
    SwitchConfig::setInetVlanName(value.toString());
    value = settings.value(SwitchSettingsStrings::iptvUnicastVlanName);
    SwitchConfig::setIptvVlanName(value.toString());

    settings.endGroup();

}

QString Config::mConfigPath = "";
QString Config::mError = "";
