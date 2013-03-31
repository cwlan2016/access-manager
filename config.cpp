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
    SnmpConfigInfo::toDefault();
    SwitchConfigInfo::toDefault();
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

    settings.setValue(SnmpSettingsStrings::port, SnmpConfigInfo::port());
    settings.setValue(SnmpSettingsStrings::readCommunity,
                      SnmpConfigInfo::readCommunity());
    settings.setValue(SnmpSettingsStrings::writeCommunity,
                      SnmpConfigInfo::writeCommunity());
    settings.setValue(SnmpSettingsStrings::retries, SnmpConfigInfo::retries());
    settings.setValue(SnmpSettingsStrings::timeout, SnmpConfigInfo::timeout());
    settings.setValue(SnmpSettingsStrings::saveConfigTimeout,
                      SnmpConfigInfo::saveConfigTimeout());

    settings.endGroup();
}

void Config::parseSnmpGroup(QSettings &settings)
{
    settings.beginGroup("snmp");

    QVariant value = settings.value(SnmpSettingsStrings::port);
    SnmpConfigInfo::setPort(value.toInt());
    value = settings.value(SnmpSettingsStrings::readCommunity);
    SnmpConfigInfo::setReadCommunity(value.toString());
    value = settings.value(SnmpSettingsStrings::writeCommunity);
    SnmpConfigInfo::setWriteCommunity(value.toString());
    value = settings.value(SnmpSettingsStrings::retries);
    SnmpConfigInfo::setRetries(value.toInt());
    value = settings.value(SnmpSettingsStrings::timeout);
    SnmpConfigInfo::setTimeout(value.toInt());
    value = settings.value(SnmpSettingsStrings::saveConfigTimeout);
    SnmpConfigInfo::setSaveConfigTimeout(value.toInt());

    settings.endGroup();
}

void Config::createSwitchGroup(QSettings &settings)
{
    settings.beginGroup("switch");

    settings.setValue(SwitchSettingsStrings::inetVlanName,
                      SwitchConfigInfo::inetVlanName());
    settings.setValue(SwitchSettingsStrings::iptvUnicastVlanName,
                      SwitchConfigInfo::iptvVlanName());

    settings.endGroup();
}

void Config::parseSwitchGroup(QSettings &settings)
{
    settings.beginGroup("switch");

    QVariant value = settings.value(SwitchSettingsStrings::inetVlanName);
    SwitchConfigInfo::setInetVlanName(value.toString());
    value = settings.value(SwitchSettingsStrings::iptvUnicastVlanName);
    SwitchConfigInfo::setIptvVlanName(value.toString());

    settings.endGroup();

}

QString Config::mConfigPath = QString();
QString Config::mError = QString();
