#include "adslportma5300.h"

#include <constant.h>
#include <converters.h>
#include <customsnmpfunctions.h>
#include <configs/dslamprofileconfig.h>

AdslPortMa5300::AdslPortMa5300(long index, QObject *parent) :
    AdslPort(index, parent)
{
}

void AdslPortMa5300::fillPrimaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex)
{
    if (portIndex == -1)
        portIndex = mIndex;

    snmpClient->addOid(createOidPair(Mib::ifDescr, 10, portIndex));
    snmpClient->addOid(createOidPair(Mib::ifOperStatus, 10, portIndex));
    snmpClient->addOid(createOidPair(Mib::ifAlias, 11, portIndex));
    snmpClient->addOid(createOidPair(Mib::adslLineConfProfile, 13, portIndex));
}

bool AdslPortMa5300::parsePrimaryLevelPdu(SnmpClient::Ptr snmpClient)
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
    mProfile = DslamProfileConfig::adsl(DeviceModel::MA5300)->displayProfileName(mProfile);
    return true;
}

void AdslPortMa5300::fillSecondaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex)
{
    if (portIndex == -1)
        portIndex = mIndex;

    snmpClient->addOid(createOidPair(Mib::ifOperStatus, 10, portIndex));
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

bool AdslPortMa5300::parseSecondaryLevelPdu(SnmpClient::Ptr snmpClient)
{
    netsnmp_variable_list *vars = snmpClient->varList();
    if (!isValidSnmpValue(vars))
        return false;

    mState = DslPortState::from(*vars->val.integer);

    if (mState == DslPortState::Up) {
        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        mTxRate = *vars->val.integer / 1000;

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        mRxRate = *vars->val.integer / 1000;
    } else {
        mTxRate = 0;
        mRxRate = 0;
        //skip vars
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
    mProfile = DslamProfileConfig::adsl(DeviceModel::MA5300)->displayProfileName(mProfile);

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
        mTxAttenuation = "";
        mRxAttenuation = "";
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
