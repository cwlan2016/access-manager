#include "switchdes3528.h"

SwitchDes3528::SwitchDes3528(QObject *parent) :
    Switch(parent)
{
}

SwitchDes3528::SwitchDes3528(QString name, QString ip, QObject *parent) :
    Switch(name, ip, parent)
{
}

SwitchDes3528::SwitchDes3528(Device *source, QObject *parent) :
    Switch(source, parent)
{
}

DeviceModel::Enum SwitchDes3528::deviceModel() const
{
    return DeviceModel::DES3528;
}

int SwitchDes3528::countPorts()
{
    return 26;
}

