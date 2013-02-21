#ifndef SWITCHINFODES3528_H
#define SWITCHINFODES3528_H

#include "switchinfo.h"

class SwitchInfoDes3528 : public SwitchInfo
{
    Q_OBJECT
public:
    explicit SwitchInfoDes3528(QObject *parent = 0);
    SwitchInfoDes3528(QString name, QString ip, QObject *parent = 0);

    DeviceModel::Enum deviceModel() const;

    void fillPdu(SnmpClient::Ptr snmpClient, int portIndex = -1);
    void parsePdu(SnmpClient::Ptr snmpClient);
};

#endif // SWITCHINFODES3528_H
