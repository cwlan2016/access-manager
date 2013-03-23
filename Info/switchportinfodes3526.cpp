#include "switchportinfodes3526.h"

#ifdef _MSC_VER
#include "../constant.h"
#include "../converters.h"
#include "../customsnmpfunctions.h"
#else
#include "constant.h"
#include "converters.h"
#include "customsnmpfunctions.h"
#endif

SwitchPortInfoDes3526::SwitchPortInfoDes3526(QObject *parent) :
    SwitchPortInfo(parent)
{
}

void SwitchPortInfoDes3526::fillPdu(SnmpClient::Ptr snmpClient, int portIndex)
{
    if (portIndex == -1)
        portIndex = mIndex;

    snmpClient->addOid(createOidPair(Mib::ifOperStatus, 10, portIndex));
    snmpClient->addOid(createOidPair(Mib::swL2PortInfoNwayStatusDES3526, 15, portIndex));
    snmpClient->addOid(createOidPair(Mib::ifAlias, 11, portIndex));
}

void SwitchPortInfoDes3526::parsePdu(SnmpClient::Ptr snmpClient)
{
    netsnmp_variable_list *vars = snmpClient->varList();
    if (!isValidSnmpValue(vars))
        return;

    mState = switchStatePortString(*vars->val.integer);

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return;

    mSpeedDuplex = speedDuplexString(DeviceModel::DES3526, *(vars->val.integer));

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return;

    mDescription = toQString(vars->val.string, vars->val_len);
}
