#include "oltlte8st.h"

#include <constant.h>
#include <customsnmpfunctions.h>

OltLte8st::OltLte8st(QObject *parent) :
    Olt(parent)
{
}

OltLte8st::OltLte8st(QString name, QString ip, QObject *parent) :
    Olt(name, ip, parent)
{
}

OltLte8st::OltLte8st(Device *source, QObject *parent) :
    Olt(source, parent)
{
}

DeviceModel::Enum OltLte8st::deviceModel() const
{
    return DeviceModel::LTE8ST;
}
