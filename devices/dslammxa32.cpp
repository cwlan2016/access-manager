#include "dslammxa32.h"

#ifdef _MSC_VER
#include "../constant.h"
#include "../customsnmpfunctions.h"
#else
#include "constant.h"
#include "customsnmpfunctions.h"
#endif

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

long DslamMxa32::snmpInterfaceIndex(BoardType::Enum boardType, int slot,
                                        int port)
{
    Q_UNUSED(boardType)
    Q_UNUSED(slot)

    return port + 1;
}

