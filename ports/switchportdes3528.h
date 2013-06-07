#ifndef SWITCHPORTDES3528_H
#define SWITCHPORTDES3528_H

#include "switchport.h"

class SwitchPortDes3528 : public SwitchPort
{
    Q_OBJECT
public:
    SwitchPortDes3528(QObject *parent);

    void fillPdu(SnmpClient::Ptr snmpClient, int portIndex = -1);
    bool parsePdu(SnmpClient::Ptr snmpClient);

protected:
    QString speedDuplexString(long snmpValue);
};

#endif // SWITCHPORTDES3528_H
