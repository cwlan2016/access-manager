#include "dslamprofileconfig.h"

#include <devices/dslamma5300.h>
#include <devices/dslamma5600.h>
#include <devices/dslammxa32.h>
#include <devices/dslammxa64.h>

void DslamProfileConfig::init()
{
    mAdslList.insert(DeviceModel::MA5300,
                     new DslProfileTableModel(DslamMa5300::defaultAdslProfiles()));
    mShdslList.insert(DeviceModel::MA5300,
                      new DslProfileTableModel(DslamMa5300::defaultShdslProfiles()));

    mAdslList.insert(DeviceModel::MA5600,
                     new DslProfileTableModel(DslamMa5600::defaultAdslProfiles()));
    mShdslList.insert(DeviceModel::MA5600,
                      new DslProfileTableModel(DslamMa5600::defaultShdslProfiles()));

    mAdslList.insert(DeviceModel::MXA32,
                     new DslProfileTableModel(DslamMxa32::defaultAdslProfiles()));
    mShdslList.insert(DeviceModel::MXA32,
                      new DslProfileTableModel(DslamMxa32::defaultShdslProfiles()));

    mAdslList.insert(DeviceModel::MXA64,
                     new DslProfileTableModel(DslamMxa64::defaultAdslProfiles()));
    mShdslList.insert(DeviceModel::MXA64,
                      new DslProfileTableModel(DslamMxa64::defaultShdslProfiles()));
}

DslProfileTableModel *DslamProfileConfig::adsl(DeviceModel::Enum deviceModel)
{
    if (mAdslList.find(deviceModel) != mAdslList.end())
        return mAdslList[deviceModel];

    return 0;
}

DslProfileTableModel *DslamProfileConfig::shdsl(DeviceModel::Enum deviceModel)
{
    if (mShdslList.find(deviceModel) != mShdslList.end())
        return mShdslList[deviceModel];

    return 0;
}

QHash<DeviceModel::Enum, DslProfileTableModel *> DslamProfileConfig::mAdslList;
QHash<DeviceModel::Enum, DslProfileTableModel *> DslamProfileConfig::mShdslList;
