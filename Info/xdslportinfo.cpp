#include "xdslportinfo.h"

XdslPortInfo::XdslPortInfo(QObject *parent) :
    QObject(parent)
{

}

int XdslPortInfo::pair() const
{
    return mPair;
}

void XdslPortInfo::setPair(int pair)
{
    mPair = pair;
}

QString XdslPortInfo::name() const
{
    return mName;
}

void XdslPortInfo::setName(QString name)
{
    mName = name;
}

DslPortState::Enum XdslPortInfo::state() const
{
    return mState;
}

void XdslPortInfo::setState(DslPortState::Enum state)
{
    mState = state;
}

QString XdslPortInfo::profile() const
{
    return mProfile;
}

void XdslPortInfo::setProfile(QString profile)
{
    mProfile = profile;
}

QString XdslPortInfo::description() const
{
    return mDescription;
}

void XdslPortInfo::setDescription(QString desc)
{
    mDescription = desc;
}

QString XdslPortInfo::rxAttenuation() const
{
    return mRxAttenuation;
}

void XdslPortInfo::setRxAttenuation(QString rxAttenuation)
{
    mRxAttenuation = rxAttenuation;
}

QString XdslPortInfo::txAttenuation() const
{
    return mTxAttenuation;
}

void XdslPortInfo::setTxAttenuation(QString txAttenuation)
{
    mTxAttenuation = txAttenuation;
}

QString XdslPortInfo::timeLastChange() const
{
    return mTimeLastChange;
}

void XdslPortInfo::setTimeLastChange(QString lastChange)
{
    mTimeLastChange = lastChange;
}
