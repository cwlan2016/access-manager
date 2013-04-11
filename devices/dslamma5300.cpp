#include "dslaminfoma5300.h"

#ifdef _MSC_VER
#include "../constant.h"
#include "../customsnmpfunctions.h"
#else
#include "constant.h"
#include "customsnmpfunctions.h"
#endif

DslamInfoMa5300::DslamInfoMa5300(QObject *parent) :
    DslamInfo(parent)
{
}

DslamInfoMa5300::DslamInfoMa5300(QString name, QString ip, QObject *parent) :
    DslamInfo(name, ip, parent)
{
}

DslamInfoMa5300::DslamInfoMa5300(DeviceInfo *source, QObject *parent) :
    DslamInfo(source, parent)
{
}

DeviceModel::Enum DslamInfoMa5300::deviceModel() const
{
    return DeviceModel::MA5300;
}

int DslamInfoMa5300::countPorts(BoardType::Enum boardType)
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

long DslamInfoMa5300::snmpInterfaceIndex(BoardType::Enum boardType, int slot,
                                         int port)
{
    if ((boardType == BoardType::AnnexA)
            || (boardType == BoardType::AnnexB)) {
        return 201326592 + 65536 * slot + 64 * port;
    } else {
        return 0;
    }
}

