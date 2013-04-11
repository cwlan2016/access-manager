#ifndef SWITCHPORTDES3550_H
#define SWITCHPORTDES3550_H

#include "switchport.h"

class SwitchPortDes3550 : public SwitchPort
{
public:
    SwitchPortDes3550(QObject *parent);

    void fillPdu(SnmpClient::Ptr snmpClient, int portIndex = -1);
    void parsePdu(SnmpClient::Ptr snmpClient);

protected:
    QString speedDuplexString(long snmpValue);
};

#endif // SWITCHPORTDES3550_H
