#include "switchdes3528.h"

#include <constant.h>
#include <customsnmpfunctions.h>
#include <ports/switchportdes3528.h>

SwitchDes3528::SwitchDes3528(QObject *parent) :
    Switch(parent)
{
}

SwitchDes3528::SwitchDes3528(QString name, QString ip, QObject *parent) :
    Switch(name, ip, parent)
{
}

SwitchDes3528::SwitchDes3528(Device *source, QObject *parent) :
    Switch(source, parent)
{
}

DeviceModel::Enum SwitchDes3528::deviceModel() const
{
    return DeviceModel::DES3528;
}

int SwitchDes3528::countPorts()
{
    return 26;
}

int SwitchDes3528::sizePortBitArray()
{
    return 32;
}

SwitchPort::Ptr SwitchDes3528::createPort(QObject *parentModel)
{
    return new SwitchPortDes3528(parentModel);
}

int SwitchDes3528::maxLengthPortDescription()
{
    return 32;
}

bool SwitchDes3528::setPortDescription(long snmpPortIndex, QString description)
{
    OidPair oid = createOidPair(Mib::ifAlias, 11, snmpPortIndex);

    return snmpSet(ip(), oid, 's', description, mError);
}

