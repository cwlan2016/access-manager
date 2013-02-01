#include "xdslportinfo.h"

XdslPortInfo::XdslPortInfo(QObject *parent) :
    QObject(parent)
{

}

int XdslPortInfo::pair() const
{
    return mPair;
}

QString XdslPortInfo::name() const
{
    return mName;
}

QString XdslPortInfo::state() const
{
    return mState;
}

QString XdslPortInfo::desc() const
{
    return mDesc;
}

QString XdslPortInfo::profile() const
{
    return mProfile;
}

QString XdslPortInfo::timeLastChange() const
{
    return mTimeLastChange;
}

QString XdslPortInfo::rxAttenuation() const
{
    return mRxAttenuation;
}

QString XdslPortInfo::txAttenuation() const
{
    return mTxAttenuation;
}

void XdslPortInfo::setPair(int pair)
{
    mPair = pair;
}

void XdslPortInfo::setName(QString name)
{
    mName = name;
}

void XdslPortInfo::setState(QString state)
{
    mState = state;
}

void XdslPortInfo::setDesc(QString desc)
{
    mDesc = desc;
}

void XdslPortInfo::setProfile(QString profile)
{
    mProfile = profile;
}

void XdslPortInfo::setTimeLastChange(QString lastChange)
{
    mTimeLastChange = lastChange;
}

void XdslPortInfo::setRxAttenuation(QString rxAttenuation)
{
    mRxAttenuation = rxAttenuation;
}

void XdslPortInfo::setTxAttenuation(QString txAttenuation)
{
    mTxAttenuation = txAttenuation;
}
