#include "switchportdes3528.h"

#include <constant.h>
#include <converters.h>
#include <customsnmpfunctions.h>

SwitchPortDes3528::SwitchPortDes3528(QObject *parent) :
    SwitchPort(parent)
{
}

void SwitchPortDes3528::fillPdu(SnmpClient::Ptr snmpClient, int portIndex)
{
    if (portIndex == -1)
        portIndex = mIndex;

    snmpClient->addOid(createOidPair(Mib::ifOperStatus, 10, portIndex));

    long numPort[] = {portIndex, 1};
    snmpClient->addOid(createOidPair(Mib::swL2PortInfoNwayStatusDES3528, 15, numPort, 2));

    if (mIndex == 25 || mIndex == 26) {
        numPort[1] = 2;
        snmpClient->addOid(createOidPair(Mib::swL2PortInfoNwayStatusDES3528, 15, numPort, 2));
    }

    snmpClient->addOid(createOidPair(Mib::ifAlias, 11, portIndex));
}

bool SwitchPortDes3528::parsePdu(SnmpClient::Ptr snmpClient)
{
    netsnmp_variable_list *vars = snmpClient->varList();
    if (!isValidSnmpValue(vars))
        return false;

    mState = SwitchPortState::from(*vars->val.integer);

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    if (mIndex == 25 || mIndex == 26) {
        QString cupperState = speedDuplexString(*vars->val.integer);

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        QString opticState = speedDuplexString(*vars->val.integer);

        mSpeedDuplex = opticState % " | " % cupperState;
    } else {
        mSpeedDuplex = speedDuplexString(*vars->val.integer);
    }

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    mDescription = toString(vars->val.string, vars->val_len);

    return true;
}

QString SwitchPortDes3528::speedDuplexString(long snmpValue)
{
    switch (snmpValue) {
    case 0:
        return "Down";
    case 6:
        return "100Mbps/Full";
    case 8:
        return "100Mbps/Half";
    case 10:
        return "1Gbps/Full";
    case 2:
        return "10Mbps/Full";
    case 4:
        return "10Mbps/Half";
    case 12:
        return "1Gbps/Half";
    default:
        return "Unknown";
    }
}

