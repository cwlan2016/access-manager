#ifndef DSLAMPROFILECONFIG_H
#define DSLAMPROFILECONFIG_H

#include <customtypes.h>
#include <models/dslprofiletablemodel.h>

class DslamProfileConfig
{
public:
    static void init();

    static DslProfileTableModel *adsl(DeviceModel::Enum deviceModel);
    static DslProfileTableModel *shdsl(DeviceModel::Enum deviceModel);

    static void toDefault();

private:
    static QHash<DeviceModel::Enum, DslProfileTableModel *> mAdslList;
    static QHash<DeviceModel::Enum, DslProfileTableModel *> mShdslList;
};

#endif // DSLAMPROFILECONFIG_H
