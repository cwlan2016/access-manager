#include "dslammxa32.h"

#include <constant.h>
#include <customsnmpfunctions.h>
#include <ports/adslportmxa32.h>

DslamMxa32::DslamMxa32(QObject *parent) :
    Dslam(parent)
{
}

DslamMxa32::DslamMxa32(QString name, QString ip, QObject *parent) :
    Dslam(name, ip, parent)
{
}

DslamMxa32::DslamMxa32(Device *source, QObject *parent) :
    Dslam(source, parent)
{
}

DeviceModel::Enum DslamMxa32::deviceModel() const
{
    return DeviceModel::MXA32;
}

int DslamMxa32::countPorts(BoardType::Enum boardType)
{
    Q_UNUSED(boardType)

    return 32;
}

long DslamMxa32::snmpPortIndex(BoardType::Enum boardType, int slot,
                                        int port)
{
    Q_UNUSED(boardType)
    Q_UNUSED(slot)

    return port + 1;
}

XdslPort::Ptr DslamMxa32::createPort(BoardType::Enum boardType, int boardIndex,
                                     int portIndex, QObject *parent)
{
    long snmpIndex = snmpPortIndex(boardType, boardIndex, portIndex);

    switch (boardType) {
    case BoardType::AnnexA:
    case BoardType::AnnexB:
        return new AdslPortMxa32(snmpIndex, parent);
    default:
        return new XdslPort(snmpIndex, parent);
    }
}

int DslamMxa32::maxLengthPortDescription()
{
    return 10;
}

bool DslamMxa32::setPortDescription(long snmpPortIndex, QString description)
{
    OidPair oid = createOidPair(Mib::mxa32DslPortName, 13, snmpPortIndex);

    return snmpSet(ip(), oid, 's', description, mError);
}

QList<DslProfile> *DslamMxa32::defaultAdslProfiles()
{
    return 0;
}

QList<DslProfile> *DslamMxa32::defaultShdslProfiles()
{
    return 0;
}

