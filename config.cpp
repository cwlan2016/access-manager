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

    QSettings settings(mConfigPath % qApp->applicationName() % ".ini", QSettings::IniFormat);
    parseSnmpGroup(settings);

    return true;
}

bool Config::save()
{
    if (exist())
        backup();

    QSettings settings(mConfigPath % qApp->applicationName() % ".ini", QSettings::IniFormat);
    createSnmpGroup(settings);
    settings.sync();

    return true;
}

bool Config::exist()
{
    QFileInfo fileInfo(mConfigPath % qApp->applicationName() % ".ini");

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
    QSettings settings(mConfigPath % qApp->applicationName() % ".ini", QSettings::IniFormat);
    createSnmpGroup(settings);
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
    settings.setValue(SnmpSettingsStrings::readCommunity, SnmpConfigInfo::readCommunity());
    settings.setValue(SnmpSettingsStrings::writeCommunity, SnmpConfigInfo::writeCommunity());
    settings.setValue(SnmpSettingsStrings::retries, SnmpConfigInfo::retries());
    settings.setValue(SnmpSettingsStrings::timeout, SnmpConfigInfo::timeout());
    settings.setValue(SnmpSettingsStrings::saveConfigTimeout, SnmpConfigInfo::saveConfigTimeout());

    settings.endGroup();
}

void Config::parseSnmpGroup(QSettings &settings)
{
    settings.beginGroup("snmp");

    SnmpConfigInfo::setPort(settings.value("port").toInt());
    SnmpConfigInfo::setReadCommunity(settings.value("readCommunity").toString());
    SnmpConfigInfo::setWriteCommunity(settings.value("writeCommunity").toString());
    SnmpConfigInfo::setRetries(settings.value("retries").toInt());
    SnmpConfigInfo::setTimeout(settings.value("timeout").toInt());
    SnmpConfigInfo::setSaveConfigTimeout(settings.value("saveConfigTimeout").toInt());

    settings.endGroup();
}

QString Config::mConfigPath = "";
QString Config::mError = "";
