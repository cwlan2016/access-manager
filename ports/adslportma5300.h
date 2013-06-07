#ifndef ADSLPORTMA5300_H
#define ADSLPORTMA5300_H

#include "adslport.h"

class AdslPortMa5300 : public AdslPort
{
    Q_OBJECT
public:
    explicit AdslPortMa5300(long index, QObject *parent = 0);

    void fillPrimaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex = -1);
    bool parsePrimaryLevelPdu(SnmpClient::Ptr snmpClient);
    void fillSecondaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex = -1);
    bool parseSecondaryLevelPdu(SnmpClient::Ptr snmpClient);
};

#endif // ADSLPORTMA5300_H
