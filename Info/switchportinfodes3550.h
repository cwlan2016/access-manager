#ifndef SWITCHPORTINFODES3550_H
#define SWITCHPORTINFODES3550_H

#include "switchportinfo.h"

class SwitchPortInfoDes3550 : public SwitchPortInfo
{
public:
    SwitchPortInfoDes3550(QObject *parent);

    void fillPdu(SnmpClient::Ptr snmpClient, int portIndex = -1);
    void parsePdu(SnmpClient::Ptr snmpClient);
};

#endif // SWITCHPORTINFODES3550_H
