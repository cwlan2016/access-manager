#include "shdslportma5600.h"

#include <constant.h>
#include <converters.h>
#include <customsnmpfunctions.h>
#include <configs/dslamprofileconfig.h>

ShdslPortMa5600::ShdslPortMa5600(int index, long snmpIndex, QObject *parent) :
    ShdslPort(index, snmpIndex, parent)
{
}

void ShdslPortMa5600::fillPrimaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex)
{
    if (portIndex == -1)
        portIndex = mSnmpIndex;

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
    profile = DslamProfileConfig::shdsl(DeviceModel::MA5600)->displayProfileName(profile);
    setProfile(profile);

    return true;
}

void ShdslPortMa5600::fillSecondaryLevelPdu(SnmpClient::Ptr snmpClient, long portIndex)
{
    if (portIndex == -1)
        portIndex = mSnmpIndex;

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

    setState(DslPortState::from(*vars->val.integer));

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

    vars = vars->next_variable->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    QString profile = toString(vars->val.string, vars->val_len);
    profile = DslamProfileConfig::shdsl(DeviceModel::MA5600)->displayProfileName(profile);
    setProfile(profile);

    if (state() == DslPortState::Up) {
        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        setActualRate(*vars->val.integer / 1000);

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        setMaxRate(*vars->val.integer / 1000);

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        setRxAttenuation(QString::number(*vars->val.integer));
        setTxAttenuation(QString::number(*vars->val.integer));

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        setSnrMargin(*vars->val.integer);

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        setTransmissonMode(transmissonModeString(*vars->val.integer));
    } else {
        setActualRate(0);
        setMaxRate(0);
        setSnrMargin(0);
        setTxAttenuation("");
        setRxAttenuation("");
        setTransmissonMode("");
    }

    return true;
}
