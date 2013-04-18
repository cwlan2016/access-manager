#include "adslportma5300.h"

#ifdef _MSC_VER
#include "../constant.h"
#include "../converters.h"
#include "../customsnmpfunctions.h"
#else
#include "constant.h"
#include "converters.h"
#include "customsnmpfunctions.h"
#endif

AdslPortMa5300::AdslPortMa5300(long index, QObject *parent) :
    AdslPort(index, parent)
{
}

void AdslPortMa5300::fillPrimaryLevelPdu(SnmpClient::Ptr snmpClient, int portIndex)
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

    return true;
    //mProfile = profileExtName(mParentDevice->deviceModel(), profile);
}

void AdslPortMa5300::fillSecondaryLevelPdu(SnmpClient::Ptr snmpClient, int portIndex)
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
}

bool AdslPortMa5300::parseSecondaryPrimaryLevelPdu(SnmpClient::Ptr snmpClient)
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
        vars = vars->next_variable->next_variable;
        if (!isValidSnmpValue(vars))
            return false;
        //пропускаем vars
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
    //portInfo->setProfile(profileExtName(mParentDevice->deviceModel(), profile));

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
    if (!isValidSnmpValue(vars))
        return false;

    QDateTime date = QDateTime::currentDateTime();
    date = date.addSecs(-*vars->val.integer / 100);
    //TODO: VALUE = [current date] - ([sysUptime] - [this value])
    mTimeLastChange = date.toString("dd.MM.yyyy hh:mm");

    return true;
}
