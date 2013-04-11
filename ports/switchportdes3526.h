#ifndef SWITCHPORTDES3526_H
#define SWITCHPORTDES3526_H

#include "switchport.h"

class SwitchPortDes3526 : public SwitchPort
{
    Q_OBJECT
public:
    SwitchPortDes3526(QObject *parent);

    void fillPdu(SnmpClient::Ptr snmpClient, int portIndex = -1);
    void parsePdu(SnmpClient::Ptr snmpClient);

protected:
    QString speedDuplexString(long snmpValue);
};

#endif // SWITCHPORTDES3526_H
