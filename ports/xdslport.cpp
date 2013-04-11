#include "xdslport.h"

XdslPort::XdslPort(QObject *parent) :
    QObject(parent)
{

}

int XdslPort::pair() const
{
    return mPair;
}

void XdslPort::setPair(int pair)
{
    mPair = pair;
}

QString XdslPort::name() const
{
    return mName;
}

void XdslPort::setName(QString name)
{
    mName = name;
}

DslPortState::Enum XdslPort::state() const
{
    return mState;
}

void XdslPort::setState(DslPortState::Enum state)
{
    mState = state;
}

QString XdslPort::profile() const
{
    return mProfile;
}

void XdslPort::setProfile(QString profile)
{
    mProfile = profile;
}

QString XdslPort::description() const
{
    return mDescription;
}

void XdslPort::setDescription(QString desc)
{
    mDescription = desc;
}

QString XdslPort::rxAttenuation() const
{
    return mRxAttenuation;
}

void XdslPort::setRxAttenuation(QString rxAttenuation)
{
    mRxAttenuation = rxAttenuation;
}

QString XdslPort::txAttenuation() const
{
    return mTxAttenuation;
}

void XdslPort::setTxAttenuation(QString txAttenuation)
{
    mTxAttenuation = txAttenuation;
}

QString XdslPort::timeLastChange() const
{
    return mTimeLastChange;
}

void XdslPort::setTimeLastChange(QString lastChange)
{
    mTimeLastChange = lastChange;
}

void XdslPort::fillPrimaryLevelPdu(SnmpClient::Ptr snmpClient, int portIndex)
{
    Q_UNUSED(snmpClient)
    Q_UNUSED(portIndex)
}

void XdslPort::parsePrimaryLevelPdu(SnmpClient::Ptr snmpClient)
{
    Q_UNUSED(snmpClient)
}

void XdslPort::fillSecondaryLevelPdu(SnmpClient::Ptr snmpClient, int portIndex)
{
    Q_UNUSED(snmpClient)
    Q_UNUSED(portIndex)
}

void XdslPort::parseSecondaryPrimaryLevelPdu(SnmpClient::Ptr snmpClient)
{
    Q_UNUSED(snmpClient)
}
