#include "switchinfodes3528.h"

SwitchInfoDes3528::SwitchInfoDes3528(QObject *parent) :
    SwitchInfo(parent)
{
}

SwitchInfoDes3528::SwitchInfoDes3528(QString name, QString ip, QObject *parent) :
    SwitchInfo(name, ip, parent)
{
}

DeviceModel::Enum SwitchInfoDes3528::deviceModel() const
{
    return DeviceModel::DES3528;
}

