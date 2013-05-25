#ifndef ADSLPORTMXA32_H
#define ADSLPORTMXA32_H

#include "adslport.h"

class AdslPortMxa32 : public AdslPort
{
    Q_OBJECT
public:
    explicit AdslPortMxa32(long index, QObject *parent = 0);

    void fillPrimaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex = -1);
    bool parsePrimaryLevelPdu(SnmpClient::Ptr snmpClient);
    void fillSecondaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex = -1);
    bool parseSecondaryLevelPdu(SnmpClient::Ptr snmpClient);
};

#endif // ADSLPORTMXA32_H
