#include "oltinfolte8st.h"

OltInfoLte8st::OltInfoLte8st(QObject *parent) :
    OltInfo(parent)
{
}

OltInfoLte8st::OltInfoLte8st(QString name, QString ip, QObject *parent) :
    OltInfo(name, ip, parent)
{
}

DeviceModel::Enum OltInfoLte8st::deviceModel() const
{
    return DeviceModel::LTE8ST;
}

void OltInfoLte8st::fillPdu(SnmpClient::Ptr snmpClient, int index)
{
}

void OltInfoLte8st::parsePdu(SnmpClient::Ptr snmpClient)
{
}
