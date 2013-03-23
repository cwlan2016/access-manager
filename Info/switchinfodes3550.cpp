#include "switchinfodes3550.h"

SwitchInfoDes3550::SwitchInfoDes3550(QObject *parent) :
    SwitchInfo(parent)
{
}

SwitchInfoDes3550::SwitchInfoDes3550(QString name, QString ip, QObject *parent) :
    SwitchInfo(name, ip, parent)
{
}

DeviceModel::Enum SwitchInfoDes3550::deviceModel() const
{
    return DeviceModel::DES3550;
}
