#include "dslaminfoma5300.h"

DslamInfoMa5300::DslamInfoMa5300(QObject *parent) :
    DslamInfo(parent)
{
}

DslamInfoMa5300::DslamInfoMa5300(QString name, QString ip, QObject *parent) :
    DslamInfo(name, ip, parent)
{
}

DeviceModel::Enum DslamInfoMa5300::deviceModel() const
{
    return DeviceModel::MA5300;
}

void DslamInfoMa5300::fillPdu(SnmpClient::Ptr snmpClient, int portIndex)
{
}

void DslamInfoMa5300::parsePdu(SnmpClient::Ptr snmpClient)
{
}
