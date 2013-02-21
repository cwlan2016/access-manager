#include "switchinfodes3528.h"

SwitchInfoDes3528::SwitchInfoDes3528(QObject *parent) :
    SwitchInfo(parent)
{
}

SwitchInfoDes3528::SwitchInfoDes3528(QString name, QString ip, QObject *parent)
{
}

DeviceModel::Enum SwitchInfoDes3528::deviceModel() const
{
    return DeviceModel::DES3528;
}

void SwitchInfoDes3528::fillPdu(SnmpClient::Ptr snmpClient, int portIndex)
{
}

void SwitchInfoDes3528::parsePdu(SnmpClient::Ptr snmpClient)
{
}
