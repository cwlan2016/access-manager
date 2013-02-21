#include "dslaminfoma5600.h"

DslamInfoMa5600::DslamInfoMa5600(QObject *parent) :
    DslamInfo(parent)
{
}

DslamInfoMa5600::DslamInfoMa5600(QString name, QString ip, QObject *parent) :
    DslamInfo(name, ip, parent)
{
}

DeviceModel::Enum DslamInfoMa5600::deviceModel() const
{
    return DeviceModel::MA5600;
}

void DslamInfoMa5600::fillPdu(SnmpClient::Ptr snmpClient, int portIndex)
{
}

void DslamInfoMa5600::parsePdu(SnmpClient::Ptr snmpClient)
{
}
