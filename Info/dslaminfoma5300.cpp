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

DeviceModel::Enum DslamInfoMa5300::deviceModel() const
{
    return DeviceModel::MA5300;
}
