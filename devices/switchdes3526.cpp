#include "switchdes3526.h"
#include <ports/switchportdes3526.h>

SwitchDes3526::SwitchDes3526(QObject *parent) :
    Switch(parent)
{
}

SwitchDes3526::SwitchDes3526(QString name, QString ip, QObject *parent) :
    Switch(name, ip, parent)
{
}

SwitchDes3526::SwitchDes3526(Device *source, QObject *parent) :
    Switch(source, parent)
{
}

DeviceModel::Enum SwitchDes3526::deviceModel() const
{
    return DeviceModel::DES3526;
}

int SwitchDes3526::countPorts()
{
    return 26;
}

SwitchPort::Ptr SwitchDes3526::createPort(QObject *parentModel)
{
    return new SwitchPortDes3526(parentModel);
}

