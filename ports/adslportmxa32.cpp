#include "adslportmxa32.h"

#include <constant.h>
#include <converters.h>
#include <customsnmpfunctions.h>
#include <configs/dslamprofileconfig.h>

AdslPortMxa32::AdslPortMxa32(int index, long snmpIndex, QObject *parent) :
    AdslPort(index, snmpIndex, parent)
{
}

void AdslPortMxa32::fillPrimaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex)
{
    if (portIndex == -1)
        portIndex = mSnmpIndex;

    snmpClient->addOid(createOidPair(Mib::mxa32DslPortOperStatus, 13, portIndex));
    snmpClient->addOid(createOidPair(Mib::mxa32DslPortAdminStatus, 13, portIndex));
    snmpClient->addOid(createOidPair(Mib::mxa32DslPortName, 13, portIndex));
    snmpClient->addOid(createOidPair(Mib::mxa32DslPortActiveProfile, 13, portIndex));
}

bool AdslPortMxa32::parsePrimaryLevelPdu(SnmpClient::Ptr snmpClient)
{
    setName(QString("adsl %1").arg(mSnmpIndex));

    netsnmp_variable_list *vars = snmpClient->varList();

    if (isValidSnmpValue(vars)
            && isValidSnmpValue(vars->next_variable)) {
        DslPortState::Enum operStatus = DslPortState::from(*vars->val.integer);
        DslPortState::Enum adminStatus = DslPortState::from(*vars->next_variable->val.integer);

        setState(DslPortState::from(operStatus, adminStatus));
    } else {
        return false;
    }

    vars = vars->next_variable->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    setDescription(toString(vars->val.string, vars->val_len));

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    QString profile = QString::number(*vars->val.integer);
    profile = DslamProfileConfig::adsl(DeviceModel::MXA32)->displayProfileName(profile);
    setProfile(profile);

    return true;
}

void AdslPortMxa32::fillSecondaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex)
{
    if (portIndex == -1)
        portIndex = mSnmpIndex;

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

        setState(DslPortState::from(operStatus, adminStatus));
    } else {
        return false;
    }

    if (state() != DslPortState::Up) {
        setTxCurrRate(0);
        setRxCurrRate(0);
        setTxAttainableRate(0);
        setRxAttainableRate(0);
        setTxAttenuation("0");
        setRxAttenuation("0");
        //TODO: params changed from status moved down. remove this recycled
        vars = vars->next_variable->next_variable->next_variable->next_variable->next_variable;
        vars = vars->next_variable->next_variable;
    }
    else {
        vars = vars->next_variable->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        setTxAttainableRate(*vars->val.integer / 1000);

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        setRxAttainableRate(*vars->val.integer / 1000);

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        setTxCurrRate(*vars->val.integer / 1000);

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        setRxCurrRate(*vars->val.integer / 1000);

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        setTxAttenuation(QString::number(*vars->val.integer));

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        setRxAttenuation(QString::number(*vars->val.integer));
    }

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    QString profile = QString::number(*vars->val.integer);
    profile = DslamProfileConfig::adsl(DeviceModel::MXA32)->displayProfileName(profile);
    setProfile(profile);

    setTxPrevRate(0); //not supported this model of dslam
    setRxPrevRate(0); //not supported this model of dslam

    return true;
}
