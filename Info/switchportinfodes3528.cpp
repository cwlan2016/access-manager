#include "switchportinfodes3528.h"

#ifdef _MSC_VER
#include "../constant.h"
#include "../converters.h"
#include "../customsnmpfunctions.h"
#else
#include "constant.h"
#include "converters.h"
#include "customsnmpfunctions.h"
#endif

SwitchPortInfoDes3528::SwitchPortInfoDes3528(QObject *parent) :
    SwitchPortInfo(parent)
{
}

void SwitchPortInfoDes3528::fillPdu(SnmpClient::Ptr snmpClient, int portIndex)
{
    if (portIndex == -1)
        portIndex = mIndex;

    snmpClient->addOid(createOidPair(Mib::ifOperStatus, 10, portIndex));

    long numPort[] = {portIndex, 1};
    snmpClient->addOid(createOidPair(Mib::swL2PortInfoNwayStatusDES3528, 15, numPort, 2, 1));

    if (mIndex == 25 || mIndex == 26) {
        numPort[1] = 2;
        snmpClient->addOid(createOidPair(Mib::swL2PortInfoNwayStatusDES3528, 15, numPort, 2, 1));
    }

    snmpClient->addOid(createOidPair(Mib::ifAlias, 11, portIndex));
}

void SwitchPortInfoDes3528::parsePdu(SnmpClient::Ptr snmpClient)
{
    netsnmp_variable_list *vars = snmpClient->varList();
    if (!isValidSnmpValue(vars))
        return;

    mState = switchStatePortString(*vars->val.integer);

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return;

    if (mIndex == 25 || mIndex == 26) {
        QString cupperState = speedDuplexString(DeviceModel::DES3528, *vars->val.integer);

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return;

        QString opticState = speedDuplexString(DeviceModel::DES3528, *(vars->val.integer));

        mSpeedDuplex = opticState % " | " % cupperState;
    } else {
        mSpeedDuplex = speedDuplexString(DeviceModel::DES3528, *(vars->val.integer));
    }

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return;

    mDescription = toQString(vars->val.string, vars->val_len);
}
