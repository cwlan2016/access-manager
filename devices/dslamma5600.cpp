#include "dslamma5600.h"

#include <constant.h>
#include <customsnmpfunctions.h>
#include <ports/adslportma5600.h>

DslamMa5600::DslamMa5600(QObject *parent) :
    Dslam(parent)
{
}

DslamMa5600::DslamMa5600(QString name, QString ip, QObject *parent) :
    Dslam(name, ip, parent)
{
}

DslamMa5600::DslamMa5600(Device *source, QObject *parent) :
    Dslam(source, parent)
{
}

DeviceModel::Enum DslamMa5600::deviceModel() const
{
    return DeviceModel::MA5600;
}

int DslamMa5600::countPorts(BoardType::Enum boardType)
{
    if ((boardType == BoardType::AnnexA)
            || (boardType == BoardType::AnnexB)) {
        return 64;
    } else if (boardType == BoardType::Shdsl) {
        return 32;
    } else {
        return 0;
    }
}

long DslamMa5600::snmpInterfaceIndex(BoardType::Enum boardType, int slot,
                                         int port)
{
    if ((boardType == BoardType::AnnexA)
            || (boardType == BoardType::AnnexB)) {
        return 201326592 + 8192 * slot + 64 * port;
    } else {
        return 0;
    }
}

XdslPort::Ptr DslamMa5600::createPort(BoardType::Enum boardType, int boardIndex,
                                      int portIndex, QObject *parent)
{
    int snmpPortIndex = snmpInterfaceIndex(boardType, boardIndex, portIndex);

    switch (boardType) {
    case BoardType::AnnexA:
    case BoardType::AnnexB:
        return new AdslPortMa5600(snmpPortIndex, parent);
    default:
        return new XdslPort(snmpPortIndex, parent);
    }
}


