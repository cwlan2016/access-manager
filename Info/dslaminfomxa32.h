#ifndef DSLAMINFOMXA32_H
#define DSLAMINFOMXA32_H

#include "dslaminfo.h"

class DslamInfoMxa32 : public DslamInfo
{
    Q_OBJECT
public:
    explicit DslamInfoMxa32(QObject *parent = 0);
    DslamInfoMxa32(QString name, QString ip, QObject *parent = 0);

    DeviceModel::Enum deviceModel() const;

    void fillPdu(SnmpClient::Ptr snmpClient, int portIndex = -1);
    void parsePdu(SnmpClient::Ptr snmpClient);
};

#endif // DSLAMINFOMXA32_H
