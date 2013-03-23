#ifndef SWITCHPORTINFODES3528_H
#define SWITCHPORTINFODES3528_H

#include "switchportinfo.h"

class SwitchPortInfoDes3528 : public SwitchPortInfo
{
    Q_OBJECT
public:
    SwitchPortInfoDes3528(QObject *parent);

    void fillPdu(SnmpClient::Ptr snmpClient, int portIndex = -1);
    void parsePdu(SnmpClient::Ptr snmpClient);
};

#endif // SWITCHPORTINFODES3528_H
