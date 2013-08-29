#include "dslammxa64.h"

#include <constant.h>
#include <customsnmpfunctions.h>
#include <ports/adslportmxa64.h>

DslamMxa64::DslamMxa64(QObject *parent) :
    Dslam(parent)
{
}

DslamMxa64::DslamMxa64(QString name, QString ip, QObject *parent) :
    Dslam(name, ip, parent)
{
}

DslamMxa64::DslamMxa64(Device *source, QObject *parent) :
    Dslam(source, parent)
{

}

DeviceModel::Enum DslamMxa64::deviceModel() const
{
    return DeviceModel::MXA64;
}

int DslamMxa64::countPorts(BoardType::Enum boardType)
{
    Q_UNUSED(boardType)

    return 64;
}

long DslamMxa64::snmpPortIndex(BoardType::Enum boardType, int slot,
                                        int port)
{
    Q_UNUSED(boardType)
    Q_UNUSED(slot)

    return port + 1;
}

XdslPort::Ptr DslamMxa64::createPort(BoardType::Enum boardType, int boardIndex,
                                     int portIndex, QObject *parent)
{
    long snmpIndex = snmpPortIndex(boardType, boardIndex, portIndex);

    switch (boardType) {
    case BoardType::AnnexA:
    case BoardType::AnnexB:
        return new AdslPortMxa64(snmpIndex, parent);
    default:
        return new XdslPort(snmpIndex, parent);
    }
}

int DslamMxa64::maxLengthPortDescription()
{
    return 10;
}

bool DslamMxa64::setPortDescription(long snmpPortIndex, QString description)
{
    OidPair oid = createOidPair(Mib::mxa64DslPortName, 13, snmpPortIndex);

    return snmpSet(ip(), oid, 's', description, mError);
}

QList<DslProfile> *DslamMxa64::defaultAdslProfiles()
{
    return 0;
}

QList<DslProfile> *DslamMxa64::defaultShdslProfiles()
{
    return 0;
}
