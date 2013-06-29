#include "shdslportma5600.h"

#include <constant.h>
#include <converters.h>
#include <customsnmpfunctions.h>
#include <configs/dslamprofileconfig.h>

ShdslPortMa5600::ShdslPortMa5600(long index, QObject *parent) :
    ShdslPort(index, parent)
{
}

void ShdslPortMa5600::fillPrimaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex)
{
    if (portIndex == -1)
        portIndex = mIndex;

    snmpClient->addOid(createOidPair(Mib::ifName, 11, portIndex));
    snmpClient->addOid(createOidPair(Mib::ifOperStatus, 10, portIndex));
    snmpClient->addOid(createOidPair(Mib::ifAlias, 11, portIndex));
    snmpClient->addOid(createOidPair(Mib::hdsl2ShdslSpanConfProfile, 12, portIndex));
}

bool ShdslPortMa5600::parsePrimaryLevelPdu(SnmpClient::Ptr snmpClient)
{
    netsnmp_variable_list *vars = snmpClient->varList();
    if (!isValidSnmpValue(vars))
        return false;

    mName = toString(vars->val.string, vars->val_len);

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    mState = DslPortState::from(*vars->val.integer);

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    mDescription = toString(vars->val.string, vars->val_len);

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    mProfile = toString(vars->val.string, vars->val_len);
    mProfile = DslamProfileConfig::shdsl(DeviceModel::MA5600)->displayProfileName(mProfile);

    return true;
}

void ShdslPortMa5600::fillSecondaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex)
{
    if (portIndex == -1)
        portIndex = mIndex;

    snmpClient->addOid(createOidPair(Mib::ifOperStatus, 10, portIndex));
    snmpClient->addOid(createOidPair(Mib::ifLastChange, 10, portIndex));
    snmpClient->addOid(createOidPair(Mib::sysUpTime, 9));
    snmpClient->addOid(createOidPair(Mib::hdsl2ShdslSpanConfProfile, 12, portIndex));
    snmpClient->addOid(createOidPair(Mib::hdsl2ShdslStatusActualLineRate, 12, portIndex));
    snmpClient->addOid(createOidPair(Mib::hdsl2ShdslStatusMaxAttainableLineRate, 12, portIndex));
    long numPort[] = { portIndex, 1, 2, 1 };
    snmpClient->addOid(createOidPair(Mib::hdsl2ShdslEndpointCurrAtn, 12, numPort, 4));
    snmpClient->addOid(createOidPair(Mib::hdsl2ShdslEndpointCurrSnrMgn, 12, numPort, 4));
    snmpClient->addOid(createOidPair(Mib::hdsl2ShdslStatusTransmissionModeCurrent, 12, portIndex));
}

bool ShdslPortMa5600::parseSecondaryLevelPdu(SnmpClient::Ptr snmpClient)
{
    netsnmp_variable_list *vars = snmpClient->varList();
    if (!isValidSnmpValue(vars))
        return false;

    mState = DslPortState::from(*vars->val.integer);

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars) || !isValidSnmpValue(vars->next_variable))
        return false;

    QDateTime date = QDateTime::currentDateTime();
    long lastChangeStatus = *vars->val.integer / 100;
    long sysUpTime = *vars->next_variable->val.integer / 100;

    if (lastChangeStatus  == 0) {
        date = date.addSecs(-sysUpTime);
    } else {
        date = date.addSecs(-lastChangeStatus);
    }

    mTimeLastChange = date.toString("dd.MM.yyyy hh:mm");

    vars = vars->next_variable->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    mProfile = toString(vars->val.string, vars->val_len);
    mProfile = DslamProfileConfig::shdsl(DeviceModel::MA5600)->displayProfileName(mProfile);

    if (mState == DslPortState::Up) {
        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        mActualRate = *vars->val.integer / 1000;

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        mMaxRate = *vars->val.integer / 1000;

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        mRxAttenuation = QString::number(*vars->val.integer);
        mTxAttenuation = QString::number(*vars->val.integer);

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        mSnrMargin = *vars->val.integer;

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        mTransmissonMode = transmissonModeString(*vars->val.integer);
    } else {
        mActualRate = 0;
        mMaxRate = 0;
        mSnrMargin = 0;
        mTxAttenuation = "";
        mRxAttenuation = "";
        mTransmissonMode = "";
    }

    return true;
}
