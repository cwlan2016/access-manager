#include "switchinfodes3526.h"

SwitchInfoDes3526::SwitchInfoDes3526(QObject *parent) :
    SwitchInfo(parent)
{
}

SwitchInfoDes3526::SwitchInfoDes3526(QString name, QString ip, QObject *parent) :
    SwitchInfo(name, ip, parent)
{
}

SwitchInfoDes3526::SwitchInfoDes3526(DeviceInfo *source, QObject *parent) :
    SwitchInfo(source, parent)
{
}

DeviceModel::Enum SwitchInfoDes3526::deviceModel() const
{
    return DeviceModel::DES3526;
}

int SwitchInfoDes3526::countPorts()
{
    return 26;
}
