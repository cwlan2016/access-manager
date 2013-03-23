#include "switchinfodes3526.h"

SwitchInfoDes3526::SwitchInfoDes3526(QObject *parent) :
    SwitchInfo(parent)
{
}

SwitchInfoDes3526::SwitchInfoDes3526(QString name, QString ip, QObject *parent) :
    SwitchInfo(name, ip, parent)
{
}

DeviceModel::Enum SwitchInfoDes3526::deviceModel() const
{
    return DeviceModel::DES3526;
}
