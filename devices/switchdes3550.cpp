#include "switchdes3550.h"
#include <ports/switchportdes3550.h>

SwitchDes3550::SwitchDes3550(QObject *parent) :
    Switch(parent)
{
}

SwitchDes3550::SwitchDes3550(QString name, QString ip, QObject *parent) :
    Switch(name, ip, parent)
{
}

SwitchDes3550::SwitchDes3550(Device *source, QObject *parent) :
    Switch(source, parent)
{
}

DeviceModel::Enum SwitchDes3550::deviceModel() const
{
    return DeviceModel::DES3550;
}

int SwitchDes3550::countPorts()
{
    return 50;
}

SwitchPort::Ptr SwitchDes3550::createPort(QObject *parentModel)
{
    return new SwitchPortDes3550(parentModel);
}
