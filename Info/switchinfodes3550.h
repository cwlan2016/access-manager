#ifndef SWITCHINFODES3550_H
#define SWITCHINFODES3550_H

#include "switchinfo.h"

class SwitchInfoDes3550 : public SwitchInfo
{
    Q_OBJECT
public:
    explicit SwitchInfoDes3550(QObject *parent = 0);
    SwitchInfoDes3550(QString name, QString ip, QObject *parent = 0);

    DeviceModel::Enum deviceModel() const;

    void fillPdu(SnmpClient::Ptr snmpClient, int portIndex = -1);
    void parsePdu(SnmpClient::Ptr snmpClient);
};

#endif // SWITCHINFODES3550_H
