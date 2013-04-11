#include "ont.h"

Ont::Ont(QObject *parent) :
    QObject(parent)
{
}

QString Ont::id()
{
    return mId;
}

void Ont::setId(QString id)
{
    mId = id;
}

QString Ont::state()
{
    return mState;
}

void Ont::setState(QString state)
{
    mState = state;
}

QString Ont::description()
{
    return mDescription;
}

void Ont::setDescription(QString description)
{
    mDescription = description;
}

OntType::Enum Ont::type()
{
    return mType;
}

void Ont::setType(OntType::Enum type)
{
    mType = type;
}

QString Ont::model()
{
    return mModel;
}

void Ont::setModel(QString model)
{
    mModel = model;
}

int Ont::serviceProfile()
{
    return mServiceProfile;
}

void Ont::setServiceProfile(int index)
{
    mServiceProfile = index;
}

int Ont::multicastProfile()
{
    return mMulticastProfile;
}

void Ont::setMulticastProfile(int index)
{
    mMulticastProfile = index;
}

void Ont::Reconfigure()
{
}
