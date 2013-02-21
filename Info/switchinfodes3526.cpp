#include "switchinfodes3526.h"

SwitchInfoDes3526::SwitchInfoDes3526(QObject *parent) :
    SwitchInfo(parent)
{
}

SwitchInfoDes3526::SwitchInfoDes3526(QString name, QString ip, QObject *parent)
{
}

DeviceModel::Enum SwitchInfoDes3526::deviceModel() const
{
    return DeviceModel::DES3526;
}

void SwitchInfoDes3526::fillPdu(SnmpClient::Ptr snmpClient, int portIndex)
{
}

void SwitchInfoDes3526::parsePdu(SnmpClient::Ptr snmpClient)
{
}
