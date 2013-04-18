#include "dslamma5300.h"

#include <ports/adslportma5300.h>

#ifdef _MSC_VER
#include "../constant.h"
#include "../customsnmpfunctions.h"
#else
#include "constant.h"
#include "customsnmpfunctions.h"
#endif

DslamMa5300::DslamMa5300(QObject *parent) :
    Dslam(parent)
{
}

DslamMa5300::DslamMa5300(QString name, QString ip, QObject *parent) :
    Dslam(name, ip, parent)
{
}

DslamMa5300::DslamMa5300(Device *source, QObject *parent) :
    Dslam(source, parent)
{
}

DeviceModel::Enum DslamMa5300::deviceModel() const
{
    return DeviceModel::MA5300;
}

int DslamMa5300::countPorts(BoardType::Enum boardType)
{
    if ((boardType == BoardType::AnnexA)
            || (boardType == BoardType::AnnexB)) {
        return 48;
    } else if (boardType == BoardType::Shdsl) {
        return 24;
    } else {
        return 0;
    }
}

long DslamMa5300::snmpInterfaceIndex(BoardType::Enum boardType, int slot,
                                         int port)
{
    if ((boardType == BoardType::AnnexA)
            || (boardType == BoardType::AnnexB)) {
        return 201326592 + 65536 * slot + 64 * port;
    } else {
        return 0;
    }
}

XdslPort::Ptr DslamMa5300::createPort(BoardType::Enum boardType, int boardIndex,
                                      int portIndex, QObject *parent)
{
    int snmpPortIndex = snmpInterfaceIndex(boardType, boardIndex, portIndex);

    switch (boardType) {
    case BoardType::AnnexA:
    case BoardType::AnnexB:
        return new AdslPortMa5300(snmpPortIndex, parent);
    default:
        return new XdslPort(snmpPortIndex, parent);
    }
}
