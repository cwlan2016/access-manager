#include "oltlte8st.h"

#ifdef _MSC_VER
#include "../constant.h"
#include "../customsnmpfunctions.h"
#else
#include "constant.h"
#include "customsnmpfunctions.h"
#endif

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
