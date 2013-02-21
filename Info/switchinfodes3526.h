#ifndef SWITCHINFODES3526_H
#define SWITCHINFODES3526_H

#include "switchinfo.h"

class SwitchInfoDes3526 : public SwitchInfo
{
    Q_OBJECT
public:
    explicit SwitchInfoDes3526(QObject *parent = 0);
    SwitchInfoDes3526(QString name, QString ip, QObject *parent = 0);

    DeviceModel::Enum deviceModel() const;

    void fillPdu(SnmpClient::Ptr snmpClient, int portIndex = -1);
    void parsePdu(SnmpClient::Ptr snmpClient);
};

#endif // SWITCHINFODES3526_H
