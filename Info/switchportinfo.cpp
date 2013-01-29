#include "switchportinfo.h"

SwitchPortInfo::SwitchPortInfo()
{
}

int SwitchPortInfo::number() const
{
    return mNumber;
}

QString SwitchPortInfo::state() const
{
    return mState;
}

QString SwitchPortInfo::desc() const
{
    return mDesc;
}

QString SwitchPortInfo::speedDuplex() const
{
    return mSpeedDuplex;
}

void SwitchPortInfo::setNumber(int number)
{
    mNumber = number;
}

void SwitchPortInfo::setState(QString state)
{
    mState = state;
}

void SwitchPortInfo::setDesc(QString desc)
{
    mDesc = desc;
}

void SwitchPortInfo::setSpeedDuplex(QString speedDuplex)
{
    mSpeedDuplex = speedDuplex;
}
