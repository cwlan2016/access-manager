#ifndef SHDSLPORTMA5300_H
#define SHDSLPORTMA5300_H

#include "shdslport.h"

class ShdslPortMa5300 : public ShdslPort
{
    Q_OBJECT
public:
    explicit ShdslPortMa5300(long index, QObject *parent = 0);

    void fillPrimaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex = -1);
    bool parsePrimaryLevelPdu(SnmpClient::Ptr snmpClient);
    void fillSecondaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex = -1);
    bool parseSecondaryLevelPdu(SnmpClient::Ptr snmpClient);
};

#endif // SHDSLPORTMA5300_H
