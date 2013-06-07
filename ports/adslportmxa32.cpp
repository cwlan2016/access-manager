#include "adslportmxa32.h"

#include <constant.h>
#include <converters.h>
#include <customsnmpfunctions.h>
#include <configs/dslamprofileconfig.h>

AdslPortMxa32::AdslPortMxa32(long index, QObject *parent) :
    AdslPort(index, parent)
{
}

void AdslPortMxa32::fillPrimaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex)
{
    if (portIndex == -1)
        portIndex = mIndex;

    snmpClient->addOid(createOidPair(Mib::mxa32DslPortOperStatus, 13, portIndex));
    snmpClient->addOid(createOidPair(Mib::mxa32DslPortAdminStatus, 13, portIndex));
    snmpClient->addOid(createOidPair(Mib::mxa32DslPortName, 13, portIndex));
    snmpClient->addOid(createOidPair(Mib::mxa32DslPortActiveProfile, 13, portIndex));
}

bool AdslPortMxa32::parsePrimaryLevelPdu(SnmpClient::Ptr snmpClient)
{
    mName = QString("adsl %1").arg(mIndex);

    netsnmp_variable_list *vars = snmpClient->varList();

    if (isValidSnmpValue(vars)
            && isValidSnmpValue(vars->next_variable)) {
        DslPortState::Enum operStatus = DslPortState::from(*vars->val.integer);
        DslPortState::Enum adminStatus = DslPortState::from(*vars->next_variable->val.integer);

        mState = DslPortState::from(operStatus, adminStatus);
    } else {
        return false;
    }

    vars = vars->next_variable->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    mDescription = toString(vars->val.string, vars->val_len);

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    mProfile = QString::number(*vars->val.integer);
    mProfile = DslamProfileConfig::adsl(DeviceModel::MXA32)->displayProfileName(mProfile);

    return true;
}

void AdslPortMxa32::fillSecondaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex)
{
    if (portIndex == -1)
        portIndex = mIndex;

    snmpClient->addOid(createOidPair(Mib::mxa32DslPortOperStatus, 13, portIndex));
    snmpClient->addOid(createOidPair(Mib::mxa32DslPortAdminStatus, 13, portIndex));
    snmpClient->addOid(createOidPair(Mib::mxa32DslBandActualRateTx, 14, portIndex));
    snmpClient->addOid(createOidPair(Mib::mxa32DslBandActualRateRx, 14, portIndex));
    snmpClient->addOid(createOidPair(Mib::mxa32DslBandLineAttenuationTx, 14, portIndex));
    snmpClient->addOid(createOidPair(Mib::mxa32DslBandLineAttenuationRx, 14, portIndex));
    snmpClient->addOid(createOidPair(Mib::mxa32DslPortActiveProfile, 13, portIndex));
}

bool AdslPortMxa32::parseSecondaryLevelPdu(SnmpClient::Ptr snmpClient)
{
    netsnmp_variable_list *vars = snmpClient->varList();

    if (isValidSnmpValue(vars)
            && isValidSnmpValue(vars->next_variable)) {
        DslPortState::Enum operStatus = DslPortState::from(*vars->val.integer);
        DslPortState::Enum adminStatus = DslPortState::from(*vars->next_variable->val.integer);

        mState = DslPortState::from(operStatus, adminStatus);
    } else {
        return false;
    }

    if (mState != DslPortState::Up) {
        mTxRate = 0;
        mRxRate = 0;
        mTxAttenuation = "";
        mRxAttenuation = "";
        vars = vars->next_variable->next_variable->next_variable->next_variable->next_variable;
    }
    else {
        vars = vars->next_variable->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        mTxRate = *vars->val.integer / 1000;

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        mRxRate = *vars->val.integer / 1000;

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        mTxAttenuation = QString::number(*vars->val.integer);

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        mRxAttenuation = QString::number(*vars->val.integer);
    }

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    mProfile = QString::number(*vars->val.integer);
    mProfile = DslamProfileConfig::adsl(DeviceModel::MXA32)->displayProfileName(mProfile);

    return true;
}
