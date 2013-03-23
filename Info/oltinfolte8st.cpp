#include "oltinfolte8st.h"

#ifdef _MSC_VER
#include "../constant.h"
#include "../customsnmpfunctions.h"
#else
#include "constant.h"
#include "customsnmpfunctions.h"
#endif

OltInfoLte8st::OltInfoLte8st(QObject *parent) :
    OltInfo(parent)
{
}

OltInfoLte8st::OltInfoLte8st(QString name, QString ip, QObject *parent) :
    OltInfo(name, ip, parent)
{
}

DeviceModel::Enum OltInfoLte8st::deviceModel() const
{
    return DeviceModel::LTE8ST;
}
