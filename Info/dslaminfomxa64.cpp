#include "dslaminfomxa64.h"

DslamInfoMxa64::DslamInfoMxa64(QObject *parent) :
    DslamInfo(parent)
{
}

DslamInfoMxa64::DslamInfoMxa64(QString name, QString ip, QObject *parent) :
    DslamInfo(name, ip, parent)
{
}

DeviceModel::Enum DslamInfoMxa64::deviceModel() const
{
    return DeviceModel::MXA64;
}

void DslamInfoMxa64::fillPdu(SnmpClient::Ptr snmpClient, int portIndex)
{
}

void DslamInfoMxa64::parsePdu(SnmpClient::Ptr snmpClient)
{
}
