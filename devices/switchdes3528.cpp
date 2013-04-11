#include "switchinfodes3528.h"

SwitchInfoDes3528::SwitchInfoDes3528(QObject *parent) :
    SwitchInfo(parent)
{
}

SwitchInfoDes3528::SwitchInfoDes3528(QString name, QString ip, QObject *parent) :
    SwitchInfo(name, ip, parent)
{
}

SwitchInfoDes3528::SwitchInfoDes3528(DeviceInfo *source, QObject *parent) :
    SwitchInfo(source, parent)
{
}

DeviceModel::Enum SwitchInfoDes3528::deviceModel() const
{
    return DeviceModel::DES3528;
}

int SwitchInfoDes3528::countPorts()
{
    return 26;
}

