#ifndef DSLAMINFOMA5300_H
#define DSLAMINFOMA5300_H

#include "dslaminfo.h"

class DslamInfoMa5300 : public DslamInfo
{
    Q_OBJECT
public:
    explicit DslamInfoMa5300(QObject *parent = 0);
    DslamInfoMa5300(QString name, QString ip, QObject *parent = 0);

    DeviceModel::Enum deviceModel() const;

    void fillPdu(SnmpClient::Ptr snmpClient, int portIndex = -1);
    void parsePdu(SnmpClient::Ptr snmpClient);
};

#endif // DSLAMINFOMA5300_H
