#include "dslaminfomxa64.h"

#ifdef _MSC_VER
#include "../constant.h"
#include "../customsnmpfunctions.h"
#else
#include "constant.h"
#include "customsnmpfunctions.h"
#endif

DslamInfoMxa64::DslamInfoMxa64(QObject *parent) :
    DslamInfo(parent)
{
}

DslamInfoMxa64::DslamInfoMxa64(QString name, QString ip, QObject *parent) :
    DslamInfo(name, ip, parent)
{
}

DslamInfoMxa64::DslamInfoMxa64(DeviceInfo *source, QObject *parent) :
    DslamInfo(source, parent)
{

}

DeviceModel::Enum DslamInfoMxa64::deviceModel() const
{
    return DeviceModel::MXA64;
}
