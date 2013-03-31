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
