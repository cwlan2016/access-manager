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
    return mDescription;
}

void SwitchPortInfo::setDescription(QString desc)
{
    mDescription = desc;
}

void SwitchPortInfo::fillPdu(SnmpClient::Ptr snmpClient, int portIndex)
{
    Q_UNUSED(snmpClient)
    Q_UNUSED(portIndex)
}

void SwitchPortInfo::parsePdu(SnmpClient::Ptr snmpClient)
{
    Q_UNUSED(snmpClient)
}

QString SwitchPortInfo::speedDuplex() const
{
    return mSpeedDuplex;
}

void SwitchPortInfo::setSpeedDuplex(QString speedDuplex)
{
    mSpeedDuplex = speedDuplex;
}
