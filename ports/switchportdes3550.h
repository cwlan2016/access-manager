#ifndef SWITCHPORTINFODES3550_H
#define SWITCHPORTINFODES3550_H

#include "switchportinfo.h"

class SwitchPortDes3550 : public SwitchPort
{
public:
    SwitchPortDes3550(QObject *parent);

    void fillPdu(SnmpClient::Ptr snmpClient, int portIndex = -1);
    void parsePdu(SnmpClient::Ptr snmpClient);

protected:
    QString speedDuplexString(long snmpValue);
};

#endif // SWITCHPORTINFODES3550_H
