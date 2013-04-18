#ifndef ADSLPORTMA5300_H
#define ADSLPORTMA5300_H

#include "adslport.h"

class AdslPortMa5300 : public AdslPort
{
    Q_OBJECT
public:
    explicit AdslPortMa5300(long index, QObject *parent = 0);

    void fillPrimaryLevelPdu(SnmpClient::Ptr snmpClient, int portIndex = -1);
    bool parsePrimaryLevelPdu(SnmpClient::Ptr snmpClient);
    void fillSecondaryLevelPdu(SnmpClient::Ptr snmpClient, int portIndex = -1);
    bool parseSecondaryPrimaryLevelPdu(SnmpClient::Ptr snmpClient);
};

#endif // ADSLPORTMA5300_H
