#ifndef ADSLPORTMA5600_H
#define ADSLPORTMA5600_H

#include "adslport.h"

class AdslPortMa5600 : public AdslPort
{
    Q_OBJECT
public:
    explicit AdslPortMa5600(long index, QObject *parent = 0);

    void fillPrimaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex = -1);
    bool parsePrimaryLevelPdu(SnmpClient::Ptr snmpClient);
    void fillSecondaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex = -1);
    bool parseSecondaryLevelPdu(SnmpClient::Ptr snmpClient);
};

#endif // ADSLPORTMA5600_H
