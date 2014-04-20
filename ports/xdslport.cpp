#include "xdslport.h"

XdslPort::XdslPort(int index, long snmpIndex, QObject *parent) :
    QObject(parent),
    mIndex(index),
    mSnmpIndex(snmpIndex),
    mPair(0),
    mState(DslPortState::Other)
{
}

int XdslPort::index() const
{
    return mIndex;
}

long XdslPort::snmpIndex() const
{
    return mSnmpIndex;
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

void XdslPort::fillPrimaryLevelPdu(SnmpClient::Ptr snmpClient, long snmpPortIndex)
{
    Q_UNUSED(snmpClient)
    Q_UNUSED(snmpPortIndex)
}

bool XdslPort::parsePrimaryLevelPdu(SnmpClient::Ptr snmpClient)
{
    Q_UNUSED(snmpClient)

    return false;
}

void XdslPort::fillSecondaryLevelPdu(SnmpClient::Ptr snmpClient, long snmpPortIndex)
{
    Q_UNUSED(snmpClient)
    Q_UNUSED(snmpPortIndex)
}

bool XdslPort::parseSecondaryLevelPdu(SnmpClient::Ptr snmpClient)
{
    Q_UNUSED(snmpClient)

    return false;
}
