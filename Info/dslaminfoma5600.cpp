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

DeviceModel::Enum DslamInfoMa5600::deviceModel() const
{
    return DeviceModel::MA5600;
}
