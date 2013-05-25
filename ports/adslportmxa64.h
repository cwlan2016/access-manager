#ifndef ADSLPORTMXA64_H
#define ADSLPORTMXA64_H

#include "adslport.h"

class AdslPortMxa64 : public AdslPort
{
    Q_OBJECT
public:
    explicit AdslPortMxa64(long index, QObject *parent = 0);

    void fillPrimaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex = -1);
    bool parsePrimaryLevelPdu(SnmpClient::Ptr snmpClient);
    void fillSecondaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex = -1);
    bool parseSecondaryLevelPdu(SnmpClient::Ptr snmpClient);
};

#endif // ADSLPORTMXA64_H
