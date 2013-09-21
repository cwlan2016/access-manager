#include "switchdes3526.h"

#include <constant.h>
#include <customsnmpfunctions.h>
#include <ports/switchportdes3526.h>

SwitchDes3526::SwitchDes3526(QObject *parent) :
    Switch(parent)
{
}

SwitchDes3526::SwitchDes3526(QString name, QString ip, QObject *parent) :
    Switch(name, ip, parent)
{
}

SwitchDes3526::SwitchDes3526(Device *source, QObject *parent) :
    Switch(source, parent)
{
}

DeviceModel::Enum SwitchDes3526::deviceModel() const
{
    return DeviceModel::DES3526;
}

int SwitchDes3526::countPorts()
{
    return 26;
}

int SwitchDes3526::sizePortBitArray()
{
    return 32;
}

SwitchPort::Ptr SwitchDes3526::createPort(QObject *parentModel)
{
    return new SwitchPortDes3526(parentModel);
}

int SwitchDes3526::maxLengthPortDescription()
{
    return 32;
}

bool SwitchDes3526::setPortDescription(long snmpPortIndex, QString description)
{
    OidPair oid = createOidPair(Mib::ifAlias, 11, snmpPortIndex);

    return snmpSet(ip(), oid, 's', description, mError);
}

