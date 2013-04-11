#include "switchportdes3526.h"

#ifdef _MSC_VER
#include "../constant.h"
#include "../converters.h"
#include "../customsnmpfunctions.h"
#else
#include "constant.h"
#include "converters.h"
#include "customsnmpfunctions.h"
#endif

SwitchPortDes3526::SwitchPortDes3526(QObject *parent) :
    SwitchPort(parent)
{
}

void SwitchPortDes3526::fillPdu(SnmpClient::Ptr snmpClient, int portIndex)
{
    if (portIndex == -1)
        portIndex = mIndex;

    snmpClient->addOid(createOidPair(Mib::ifOperStatus, 10, portIndex));
    snmpClient->addOid(createOidPair(Mib::swL2PortInfoNwayStatusDES3526, 15, portIndex));
    snmpClient->addOid(createOidPair(Mib::ifAlias, 11, portIndex));
}

void SwitchPortDes3526::parsePdu(SnmpClient::Ptr snmpClient)
{
    netsnmp_variable_list *vars = snmpClient->varList();
    if (!isValidSnmpValue(vars))
        return;

    mState = SwitchPortState::from(*vars->val.integer);

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return;

    mSpeedDuplex = speedDuplexString(*(vars->val.integer));

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return;

    mDescription = toString(vars->val.string, vars->val_len);
}

QString SwitchPortDes3526::speedDuplexString(long snmpValue)
{
    switch (snmpValue) {
    case 2:
        return "Auto";
    case 6:
        return "100Mbps/Full";
    case 5:
        return "100Mbps/Half";
    case 8:
        return "1Gbps/Full";
    case 4:
        return "10Mbps/Full";
    case 3:
        return "10Mbps/Half";
    case 7:
        return "1Gbps/Half";
    default:
        return "Unknown";
    }
}
