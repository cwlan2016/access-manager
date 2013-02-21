#include "dslaminfomxa32.h"

DslamInfoMxa32::DslamInfoMxa32(QObject *parent) :
    DslamInfo(parent)
{
}

DslamInfoMxa32::DslamInfoMxa32(QString name, QString ip, QObject *parent) :
    DslamInfo(name, ip, parent)
{
}

DeviceModel::Enum DslamInfoMxa32::deviceModel() const
{
    return DeviceModel::MXA32;
}

void DslamInfoMxa32::fillPdu(SnmpClient::Ptr snmpClient, int portIndex)
{
}

void DslamInfoMxa32::parsePdu(SnmpClient::Ptr snmpClient)
{
}
