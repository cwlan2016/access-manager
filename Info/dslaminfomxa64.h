#ifndef DSLAMINFOMXA64_H
#define DSLAMINFOMXA64_H

#include "dslaminfo.h"

class DslamInfoMxa64 : public DslamInfo
{
    Q_OBJECT
public:
    explicit DslamInfoMxa64(QObject *parent = 0);
    DslamInfoMxa64(QString name, QString ip, QObject *parent = 0);

    DeviceModel::Enum deviceModel() const;

    void fillPdu(SnmpClient::Ptr snmpClient, int portIndex = -1);
    void parsePdu(SnmpClient::Ptr snmpClient);
};

#endif // DSLAMINFOMXA64_H
