#include "dslaminfomxa32.h"

#ifdef _MSC_VER
#include "../constant.h"
#include "../customsnmpfunctions.h"
#else
#include "constant.h"
#include "customsnmpfunctions.h"
#endif

DslamInfoMxa32::DslamInfoMxa32(QObject *parent) :
    DslamInfo(parent)
{
}

DslamInfoMxa32::DslamInfoMxa32(QString name, QString ip, QObject *parent) :
    DslamInfo(name, ip, parent)
{
}

DslamInfoMxa32::DslamInfoMxa32(DeviceInfo *source, QObject *parent) :
    DslamInfo(source, parent)
{
}

DeviceModel::Enum DslamInfoMxa32::deviceModel() const
{
    return DeviceModel::MXA32;
}

int DslamInfoMxa32::countPorts(BoardType::Enum boardType)
{
    Q_UNUSED(boardType)

    return 32;
}

long DslamInfoMxa32::snmpInterfaceIndex(BoardType::Enum boardType, int slot,
                                        int port)
{
    Q_UNUSED(boardType)
    Q_UNUSED(slot)

    return port + 1;
}

