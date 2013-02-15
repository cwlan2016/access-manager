#include "switchportinfo.h"

SwitchPortInfo::SwitchPortInfo(QObject *parent) :
    QObject(parent)
{
}

int SwitchPortInfo::index() const
{
    return mIndex;
}

void SwitchPortInfo::setIndex(int index)
{
    mIndex = index;
}

QString SwitchPortInfo::state() const
{
    return mState;
}

void SwitchPortInfo::setState(QString state)
{
    mState = state;
}

QString SwitchPortInfo::description() const
{
    return mDesc;
}

void SwitchPortInfo::setDescription(QString desc)
{
    mDesc = desc;
}

QString SwitchPortInfo::speedDuplex() const
{
    return mSpeedDuplex;
}

void SwitchPortInfo::setSpeedDuplex(QString speedDuplex)
{
    mSpeedDuplex = speedDuplex;
}
