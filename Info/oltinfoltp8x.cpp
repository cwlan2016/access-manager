#include "oltinfoltp8x.h"

#ifdef _MSC_VER
#include "../constant.h"
#include "../customsnmpfunctions.h"
#else
#include "constant.h"
#include "customsnmpfunctions.h"
#endif

OltInfoLtp8x::OltInfoLtp8x(QObject *parent) :
    OltInfo(parent)
{
}

OltInfoLtp8x::OltInfoLtp8x(QString name, QString ip, QObject *parent) :
    OltInfo(name, ip, parent)
{
}

OltInfoLtp8x::OltInfoLtp8x(DeviceInfo *source, QObject *parent) :
    OltInfo(source, parent)
{
}

DeviceModel::Enum OltInfoLtp8x::deviceModel() const
{
    return DeviceModel::LTP8X;
}
