#include "config.h"

bool Config::exist()
{
    QFileInfo fileInfo(mConfigPath % qApp->applicationName() % ".ini");

    return fileInfo.exists();
}

void Config::toDefault()
{
    SnmpConfigInfo::toDefault();
    QSettings settings (QSettings::IniFormat,QSettings::UserScope,
                        qApp->organizationName(), qApp->applicationName());
    createSnmpGroup(settings);
}

bool Config::load()
{
    if (!exist())
    {
        toDefault();

        return true;
    }

    QSettings settings (QSettings::IniFormat,QSettings::UserScope,
                        qApp->organizationName(), qApp->applicationName());
    parseSnmpGroup(settings);

    return true;
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

bool Config::save()
{
    if (exist())
        backup();

    QSettings settings (QSettings::IniFormat,QSettings::UserScope, qApp->organizationName(), qApp->applicationName());
    createSnmpGroup(settings);

    return true;
}

QString Config::errorString()
{
    return mError;
}

void Config::initializePath()
{
    mConfigPath = QDesktopServices::storageLocation(QDesktopServices::HomeLocation)% "\\Application Data\\" %  qApp->organizationName() % "\\";
}

QString Config::path()
{
    return mConfigPath;
}

void Config::createSnmpGroup(QSettings& settings)
{
    settings.beginGroup("snmp");
    settings.setValue(SnmpSettingsString::port,SnmpConfigInfo::port());
    settings.setValue(SnmpSettingsString::readCommunity,SnmpConfigInfo::readCommunity());
    settings.setValue(SnmpSettingsString::writeCommunity,SnmpConfigInfo::writeCommunity());
    settings.setValue(SnmpSettingsString::retries,SnmpConfigInfo::retries());
    settings.setValue(SnmpSettingsString::timeout,SnmpConfigInfo::timeout());
    settings.setValue(SnmpSettingsString::saveConfigTimeout,SnmpConfigInfo::saveConfigTimeout());
    settings.endGroup();
    settings.sync();
}

void Config::parseSnmpGroup(QSettings& settings)
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
