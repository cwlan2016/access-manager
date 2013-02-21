#ifndef OLTINFOLTE8ST_H
#define OLTINFOLTE8ST_H

#include "oltinfo.h"

class OltInfoLte8st : public OltInfo
{
    Q_OBJECT
public:
    explicit OltInfoLte8st(QObject *parent = 0);
    OltInfoLte8st(QString name, QString ip, QObject *parent = 0);

    DeviceModel::Enum deviceModel() const;

    void fillPdu(SnmpClient::Ptr snmpClient, int index = -1);
    void parsePdu(SnmpClient::Ptr snmpClient);
};

#endif // OLTINFOLTE8ST_H
