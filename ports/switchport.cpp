#include "switchport.h"

SwitchPort::SwitchPort(QObject *parent) :
    QObject(parent),
    mIndex(0),
    mState(SwitchPortState::Other)
{
}

int SwitchPort::index() const
{
    return mIndex;
}

void SwitchPort::setIndex(int index)
{
    mIndex = index;
}

SwitchPortState::Enum SwitchPort::state() const
{
    return mState;
}

void SwitchPort::setState(SwitchPortState::Enum state)
{
    mState = state;
}

QString SwitchPort::description() const
{
    return mDescription;
}

void SwitchPort::setDescription(QString desc)
{
    mDescription = desc;
}

void SwitchPort::fillPdu(SnmpClient::Ptr snmpClient, int portIndex)
{
    Q_UNUSED(snmpClient)
    Q_UNUSED(portIndex)
}

bool SwitchPort::parsePdu(SnmpClient::Ptr snmpClient)
{
    Q_UNUSED(snmpClient)

    return false;
}

QString SwitchPort::speedDuplexString(long snmpValue)
{
    Q_UNUSED(snmpValue)

    return "";
}

QString SwitchPort::speedDuplex() const
{
    return mSpeedDuplex;
}

void SwitchPort::setSpeedDuplex(QString speedDuplex)
{
    mSpeedDuplex = speedDuplex;
}
