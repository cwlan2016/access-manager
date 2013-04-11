#include "adslportma5300.h"

AdslPortMa5300::AdslPortMa5300(QObject *parent) :
    AdslPort(parent)
{
}

void AdslPortMa5300::fillPrimaryLevelPdu(SnmpClient::Ptr snmpClient, int portIndex)
{
}

void AdslPortMa5300::parsePrimaryLevelPdu(SnmpClient::Ptr snmpClient)
{
}

void AdslPortMa5300::fillSecondaryLevelPdu(SnmpClient::Ptr snmpClient, int portIndex)
{
}

void AdslPortMa5300::parseSecondaryPrimaryLevelPdu(SnmpClient::Ptr snmpClient)
{
}
