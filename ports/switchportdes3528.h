#ifndef SWITCHPORTINFODES3528_H
#define SWITCHPORTINFODES3528_H

#include "switchportinfo.h"

class SwitchPortDes3528 : public SwitchPort
{
    Q_OBJECT
public:
    SwitchPortDes3528(QObject *parent);

    void fillPdu(SnmpClient::Ptr snmpClient, int portIndex = -1);
    void parsePdu(SnmpClient::Ptr snmpClient);
protected:
    QString speedDuplexString(long snmpValue);
};

#endif // SWITCHPORTINFODES3528_H
