#include "oltltp8x.h"

#include <constant.h>
#include <customsnmpfunctions.h>

OltLtp8x::OltLtp8x(QObject *parent) :
    Olt(parent)
{
}

OltLtp8x::OltLtp8x(QString name, QString ip, QObject *parent) :
    Olt(name, ip, parent)
{
}

OltLtp8x::OltLtp8x(Device *source, QObject *parent) :
    Olt(source, parent)
{
}

DeviceModel::Enum OltLtp8x::deviceModel() const
{
    return DeviceModel::LTP8X;
}
