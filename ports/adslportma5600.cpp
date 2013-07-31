#include "adslportma5600.h"

#include <constant.h>
#include <converters.h>
#include <customsnmpfunctions.h>
#include <configs/dslamprofileconfig.h>

AdslPortMa5600::AdslPortMa5600(long index, QObject *parent) :
    AdslPort(index, parent)
{
}

void AdslPortMa5600::fillPrimaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex)
{
    if (portIndex == -1)
        portIndex = mIndex;

    snmpClient->addOid(createOidPair(Mib::ifName, 11, portIndex));
    snmpClient->addOid(createOidPair(Mib::ifOperStatus, 10, portIndex));
    snmpClient->addOid(createOidPair(Mib::ifAlias, 11, portIndex));
    snmpClient->addOid(createOidPair(Mib::adslLineConfProfile, 13, portIndex));
}

bool AdslPortMa5600::parsePrimaryLevelPdu(SnmpClient::Ptr snmpClient)
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
    mProfile = DslamProfileConfig::adsl(DeviceModel::MA5600)->displayProfileName(mProfile);

    return true;
}

void AdslPortMa5600::fillSecondaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex)
{
    if (portIndex == -1)
        portIndex = mIndex;

    snmpClient->addOid(createOidPair(Mib::ifOperStatus, 10, portIndex));
    snmpClient->addOid(createOidPair(Mib::adslAtucChanPrevTxRate, 13, portIndex));
    snmpClient->addOid(createOidPair(Mib::adslAturChanPrevTxRate, 13, portIndex));
    snmpClient->addOid(createOidPair(Mib::adslAtucCurrAttainableRate, 13, portIndex));
    snmpClient->addOid(createOidPair(Mib::adslAturCurrAttainableRate, 13, portIndex));
    snmpClient->addOid(createOidPair(Mib::adslAtucChanCurrTxRate, 13, portIndex));
    snmpClient->addOid(createOidPair(Mib::adslAturChanCurrTxRate, 13, portIndex));
    snmpClient->addOid(createOidPair(Mib::adslLineCoding, 13, portIndex));
    snmpClient->addOid(createOidPair(Mib::adslLineType, 13, portIndex));
    snmpClient->addOid(createOidPair(Mib::adslLineConfProfile, 13, portIndex));
    snmpClient->addOid(createOidPair(Mib::adslAtucCurrAtn, 13, portIndex));
    snmpClient->addOid(createOidPair(Mib::adslAturCurrAtn, 13, portIndex));
    snmpClient->addOid(createOidPair(Mib::ifLastChange, 10, portIndex));
    snmpClient->addOid(createOidPair(Mib::sysUpTime, 9));
}

bool AdslPortMa5600::parseSecondaryLevelPdu(SnmpClient::Ptr snmpClient)
{
    netsnmp_variable_list *vars = snmpClient->varList();
    if (!isValidSnmpValue(vars))
        return false;

    mState = DslPortState::from(*vars->val.integer);

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    if (*vars->val.integer == 4294967295) {
        mTxPrevRate = 0;
    } else {
        mTxPrevRate = *vars->val.integer / 1000;
    }

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    if (*vars->val.integer == 4294967295) {
        mRxPrevRate = 0;
    } else {
        mRxPrevRate = *vars->val.integer / 1000;
    }

    if (mState == DslPortState::Up) {
        vars = vars->next_variable;
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
    } else {
        mTxAttainableRate = 0;
        mRxAttainableRate = 0;
        mTxCurrRate = 0;
        mRxCurrRate = 0;
        //skip vars
        vars = vars->next_variable->next_variable;
        vars = vars->next_variable->next_variable;
        if (!isValidSnmpValue(vars))
            return false;
    }

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    mCoding = codingString(*vars->val.integer);

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    mLineType = typeLineString(*vars->val.integer);

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    mProfile = toString(vars->val.string, vars->val_len);
    mProfile = DslamProfileConfig::adsl(DeviceModel::MA5600)->displayProfileName(mProfile);

    if (mState == DslPortState::Up) {
        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        mTxAttenuation = QString::number(*vars->val.integer / 10.0);

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        mRxAttenuation = QString::number(*vars->val.integer / 10.0);
    } else {
        mTxAttenuation = "0";
        mRxAttenuation = "0";
        vars = vars->next_variable->next_variable;
        if (!isValidSnmpValue(vars))
            return false;
    }

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

    return true;
}
