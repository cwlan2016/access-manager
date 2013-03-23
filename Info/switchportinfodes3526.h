#ifndef SWITCHPORTINFODES3526_H
#define SWITCHPORTINFODES3526_H

#include "switchportinfo.h"

class SwitchPortInfoDes3526 : public SwitchPortInfo
{
    Q_OBJECT
public:
    SwitchPortInfoDes3526(QObject *parent);

    void fillPdu(SnmpClient::Ptr snmpClient, int portIndex = -1);
    void parsePdu(SnmpClient::Ptr snmpClient);
};

#endif // SWITCHPORTINFODES3526_H
