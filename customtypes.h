#ifndef CUSTOMTYPES_H
#define CUSTOMTYPES_H

#include <QtCore/QString>
#include <map>

typedef std::map<int, QString> OltProfileMap;

namespace BoardType
{
enum Enum : short
{
    AnnexA =    0,
    AnnexB =    1,
    Shdsl =     2,
    Other =     3,
    Count =     4
};

const QString BoardTypeName[] = { "ADSL Annex A", "ADSL Annex B", "SHDSL", "Other" };

BoardType::Enum from(QString boardType);
QString         toString(BoardType::Enum boardType);
}

namespace DeviceModel
{
enum Enum
{
    Other =     0,
    DES3526 =   1,
    DES3528 =   2,
    DES3550 =   3,
    DES3552 =   4,
    MA5600 =    5,
    MA5300 =    6,
    MA5616 =    7,
    MXA32 =     8,
    MXA64 =     9,
    LTE8ST =    10,
    LTP8X =     11,
    Count =     12
};

const QString DeviceModelName[] = { "Other", "DES-3526", "DES-3528", "DES-3550",
                                    "DES-3552", "MA5600", "MA5300", "MA5616",
                                    "MXA-32", "MXA-64", "LTE-8ST", "LTP-8X" };

DeviceModel::Enum   from(QString deviceModel);
QString             toString(DeviceModel::Enum deviceModel);
}

namespace DeviceType
{
enum Enum : short
{
    Other =     0,
    Switch =    1,
    Dslam =     2,
    Olt =       3
};

const QString DeviceTypeName[] = { "Other", "Switch", "Dslam", "Olt" };

DeviceType::Enum    from(QString deviceType);
DeviceType::Enum    from(DeviceModel::Enum deviceModel);
QString             toString(DeviceType::Enum deviceType);
}

namespace DslPortState
{
enum Enum : short
{
    Up =            1,
    Down =          2,
    Activating =    3,
    Defective =     4,
    Other =         5
};

const QString DslPortStateName[] = { "Up", "Down", "Activating", "Defective", "Other"};

DslPortState::Enum  from(QString dslPortState);
QString             toString(DslPortState::Enum dslPortState);
}

namespace OntType {
enum Enum : short
{
    Nte = 1,
    Ntp = 2
};
}

namespace NteModel {
enum Enum : short
{
    Nte_2c =            2,
    Nte_rg_1400f =      3,
    Nte_rg_1400g =      4,
    Nte_rg_1400f_w =    5,
    Nte_rg_1400g_w =    6,
    Nte_rg_1400fc =     7,
    Nte_rg_1400gc =     8,
    Nte_rg_1400fc_w =   9,
    Nte_rg_1400gc_w =   10,
    Nte_rg_1402f =      11,
    Nte_rg_1402g =      12,
    Nte_rg_1402f_w =    13,
    Nte_rg_1402g_w =    14,
    Nte_rg_1402fc =     15,
    Nte_rg_1402gc =     16,
    Nte_rg_1402fc_w =   17,
    Nte_rg_1402gc_w =   18
};
}

namespace NteState {
enum Enum : short
{
    Free =              0,
    Allocated =         1,
    AuthInProgress =    2,
    CfgInProgress =     3,
    AuthFailed =        4,
    CfgFailed =         5,
    ReportTimeout =     6,
    Ok =                7,
    AuthOk =            8,
    ResetInProgress =   9,
    ResetOk =           10,
    Discovered =        11,
    Blocked =           12
};
}

namespace NtpState {
enum Enum : short
{
    Free =              0,
    Allocated =         1,
    AuthInProgress =    2,
    AuthFailed =        3,
    AuthOk =            4,
    CfgInProgress =     5,
    CfgFailed =         6,
    Ok =                7,
    Failed =            8,
    Blocked =           9,
    Mibreset =          10,
    Preconfig =         11,
    FwUpdating =        12,
    Unactivated =       13,
    Redundant =         14,
    Disabled =          15,
    Unknown =           16
};
}

namespace PageType
{
enum Enum
{
    DeviceListPage,
    DslamPage,
    EditDslamPage,
    SwitchPage,
    OltPage,
    AboutPage,
    SettingsPage
};
}

namespace SessionType
{
enum Enum
{
    ReadSession,
    WriteSession
};
}

namespace VlanState {
enum Enum : short
{
    Untag = 0,
    Tag =   1,
    None =  2
};
}

#endif // CUSTOMTYPES_H
