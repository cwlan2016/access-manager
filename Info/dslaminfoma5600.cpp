#include "dslaminfoma5600.h"

#ifdef _MSC_VER
#include "../constant.h"
#include "../customsnmpfunctions.h"
#else
#include "constant.h"
#include "customsnmpfunctions.h"
#endif

DslamInfoMa5600::DslamInfoMa5600(QObject *parent) :
    DslamInfo(parent)
{
}

DslamInfoMa5600::DslamInfoMa5600(QString name, QString ip, QObject *parent) :
    DslamInfo(name, ip, parent)
{
}

DslamInfoMa5600::DslamInfoMa5600(DeviceInfo *source, QObject *parent) :
    DslamInfo(source, parent)
{
}

DeviceModel::Enum DslamInfoMa5600::deviceModel() const
{
    return DeviceModel::MA5600;
}

int DslamInfoMa5600::countPorts(BoardType::Enum boardType)
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

long DslamInfoMa5600::snmpInterfaceIndex(BoardType::Enum boardType, int slot,
                                         int port)
{
    if ((boardType == BoardType::AnnexA)
            || (boardType == BoardType::AnnexB)) {
        return 201326592 + 8192 * slot + 64 * port;
    } else {
        return 0;
    }
}

