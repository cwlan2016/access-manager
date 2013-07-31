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
    snmpClient->addOid(createOidPair(Mib::mxa32DslBandAttainableRateTx, 14, portIndex));
    snmpClient->addOid(createOidPair(Mib::mxa32DslBandAttainableRateRx, 14, portIndex));
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
        mTxCurrRate = 0;
        mRxCurrRate = 0;
        mTxAttainableRate = 0;
        mRxAttainableRate = 0;
        mTxAttenuation = "0";
        mRxAttenuation = "0";
        //TODO: params changed from status moved down. remove this recycled
        vars = vars->next_variable->next_variable->next_variable->next_variable->next_variable;
        vars = vars->next_variable->next_variable;
    }
    else {
        vars = vars->next_variable->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        mTxAttainableRate = *vars->val.integer / 1000;

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        mRxAttainableRate = *vars->val.integer / 1000;

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        mTxCurrRate = *vars->val.integer / 1000;

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        mRxCurrRate = *vars->val.integer / 1000;

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

    mTxPrevRate = 0; //not supported this model of dslam
    mRxPrevRate = 0; //not supported this model of dslam

    return true;
}
