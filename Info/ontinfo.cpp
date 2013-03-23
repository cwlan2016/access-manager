#include "ontinfo.h"

OntInfo::OntInfo(QObject *parent) :
    QObject(parent)
{
}

QString OntInfo::id()
{
    return mId;
}

void OntInfo::setId(QString id)
{
    mId = id;
}

QString OntInfo::state()
{
    return mState;
}

void OntInfo::setState(QString state)
{
    mState = state;
}

QString OntInfo::description()
{
    return mDescription;
}

void OntInfo::setDescription(QString description)
{
    mDescription = description;
}

OntType::Enum OntInfo::type()
{
    return mType;
}

void OntInfo::setType(OntType::Enum type)
{
    mType = type;
}

QString OntInfo::model()
{
    return mModel;
}

void OntInfo::setModel(QString model)
{
    mModel = model;
}

int OntInfo::serviceProfile()
{
    return mServiceProfile;
}

void OntInfo::setServiceProfile(int index)
{
    mServiceProfile = index;
}

int OntInfo::multicastProfile()
{
    return mMulticastProfile;
}

void OntInfo::setMulticastProfile(int index)
{
    mMulticastProfile = index;
}

void OntInfo::Reconfigure()
{
}
