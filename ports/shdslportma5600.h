#ifndef SHDSLPORTMA5600_H
#define SHDSLPORTMA5600_H

#include "shdslport.h"

class ShdslPortMa5600 : public ShdslPort
{
    Q_OBJECT
public:
    explicit ShdslPortMa5600(int index, long snmpIndex, QObject *parent = 0);

    void fillPrimaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex = -1);
    bool parsePrimaryLevelPdu(SnmpClient::Ptr snmpClient);
    void fillSecondaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex = -1);
    bool parseSecondaryLevelPdu(SnmpClient::Ptr snmpClient);
};

#endif // SHDSLPORTMA5600_H
