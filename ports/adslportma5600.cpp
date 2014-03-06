#include "adslportma5600.h"

#include <constant.h>
#include <converters.h>
#include <customsnmpfunctions.h>
#include <configs/dslamprofileconfig.h>

AdslPortMa5600::AdslPortMa5600(int index, long snmpIndex, QObject *parent) :
    AdslPort(index, snmpIndex, parent)
{
}

void AdslPortMa5600::fillPrimaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex)
{
    if (portIndex == -1)
        portIndex = mSnmpIndex;

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

    setName(toString(vars->val.string, vars->val_len));

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    setState(DslPortState::from(*vars->val.integer));

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    setDescription(toString(vars->val.string, vars->val_len));

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    QString profile = toString(vars->val.string, vars->val_len);
    profile = DslamProfileConfig::adsl(DeviceModel::MA5600)->displayProfileName(profile);
    setProfile(profile);

    return true;
}

void AdslPortMa5600::fillSecondaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex)
{
    if (portIndex == -1)
        portIndex = mSnmpIndex;

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

    setState(DslPortState::from(*vars->val.integer));

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    if (*vars->val.integer == LONG_MAX) {
        setTxPrevRate(0);
    } else {
        setTxPrevRate(*vars->val.integer / 1000);
    }

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    if (*vars->val.integer == LONG_MAX) {
        setRxPrevRate(0);
    } else {
        setRxPrevRate(*vars->val.integer / 1000);
    }

    if (state() == DslPortState::Up) {
        vars = vars->next_variable;
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
    } else {
        setTxAttainableRate(0);
        setRxAttainableRate(0);
        setTxCurrRate(0);
        setRxCurrRate(0);
        //skip vars
        vars = vars->next_variable->next_variable;
        vars = vars->next_variable->next_variable;
        if (!isValidSnmpValue(vars))
            return false;
    }

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    setCoding(codingString(*vars->val.integer));

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    setLineType(typeLineString(*vars->val.integer));

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    QString profile = toString(vars->val.string, vars->val_len);
    profile = DslamProfileConfig::adsl(DeviceModel::MA5600)->displayProfileName(profile);
    setProfile(profile);

    if (state() == DslPortState::Up) {
        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        setTxAttenuation(QString::number(*vars->val.integer / 10.0));

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        setRxAttenuation(QString::number(*vars->val.integer / 10.0));
    } else {
        setTxAttenuation("0");
        setRxAttenuation("0");
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

    setTimeLastChange(date.toString("dd.MM.yyyy hh:mm"));

    return true;
}
