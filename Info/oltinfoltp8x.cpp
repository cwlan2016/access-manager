#include "oltinfoltp8x.h"

OltInfoLtp8x::OltInfoLtp8x(QObject *parent) :
    OltInfo(parent)
{
}

OltInfoLtp8x::OltInfoLtp8x(QString name, QString ip, QObject *parent) :
    OltInfo(name, ip, parent)
{
}

DeviceModel::Enum OltInfoLtp8x::deviceModel() const
{
    return DeviceModel::LTP8X;
}

void OltInfoLtp8x::fillPdu(SnmpClient::Ptr snmpClient, int portIndex)
{
}

void OltInfoLtp8x::parsePdu(SnmpClient::Ptr snmpClient)
{
}
