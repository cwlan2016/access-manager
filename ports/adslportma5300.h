#ifndef ADSLPORTINFOMA5300_H
#define ADSLPORTINFOMA5300_H

#include "adslportinfo.h"

class AdslPortMa5300 : public AdslPort
{
    Q_OBJECT
public:
    explicit AdslPortMa5300(QObject *parent = 0);

    void fillPrimaryLevelPdu(SnmpClient::Ptr snmpClient, int portIndex);
    void parsePrimaryLevelPdu(SnmpClient::Ptr snmpClient);
    void fillSecondaryLevelPdu(SnmpClient::Ptr snmpClient, int portIndex);
    void parseSecondaryPrimaryLevelPdu(SnmpClient::Ptr snmpClient);
};

#endif // ADSLPORTINFOMA5300_H
