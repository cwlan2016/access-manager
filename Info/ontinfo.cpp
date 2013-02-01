#include "ontinfo.h"

OntInfo::OntInfo(QObject *parent) :
    QObject(parent)
{
}

QString OntInfo::id()
{
    return mId;
}

QString OntInfo::state()
{
    return mState;
}

QString OntInfo::description()
{
    return mDescription;
}

OntType::Enum OntInfo::type()
{
    return mType;
}

QString OntInfo::model()
{
    return mModel;
}

int OntInfo::serviceProfile()
{
    return mServiceProfile;
}

int OntInfo::multicastProfile()
{
    return mMulticastProfile;
}

void OntInfo::setId(QString id)
{
    mId = id;
}

void OntInfo::setState(QString state)
{
    mState = state;
}

void OntInfo::setDescription(QString description)
{
    mDescription = description;
}

void OntInfo::setType(OntType::Enum type)
{
    mType = type;
}

void OntInfo::setModel(QString model)
{
    mModel = model;
}

void OntInfo::setServiceProfile(int index)
{
    mServiceProfile = index;
}

void OntInfo::setMulticastProfile(int index)
{
    mMulticastProfile = index;
}

void OntInfo::Reconfigure()
{
}
