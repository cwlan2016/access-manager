#ifndef CUSTOMTYPES_H
#define CUSTOMTYPES_H

#include "stdafx.h"

typedef QPair<oid *, int> OidPair;
typedef QPair<QString, QString> DslProfile;
typedef QMap<int, QString> OltProfileMap;

namespace BoardType
{
enum Enum {
    AnnexA =    0,
    AnnexB =    1,
    Shdsl =     2,
    Other =     3,
    Count =     4
};

const QString BoardTypeName[] = { "ADSL Annex A", "ADSL Annex B", "SHDSL",
                                  "Other" };

inline BoardType::Enum from(QString boardType)
{
    if (boardType == BoardTypeName[BoardType::AnnexA]) {
        return BoardType::AnnexA;
    } else if (boardType == BoardTypeName[BoardType::AnnexB]) {
        return BoardType::AnnexB;
    } else if (boardType == BoardTypeName[BoardType::Shdsl]) {
        return BoardType::Shdsl;
    } else {
        return BoardType::Other;
    }
}

inline QString toString(BoardType::Enum boardType)
{
    return BoardTypeName[boardType];
}
}

namespace HuaweiSlotType
{
enum Enum {
    //MA5600
    ADEF =      767,     // 64-port ADSL AnnexA Board
    ADEF2 =     761,     // 64-port ADSL AnnexA Board
    ADBF =      131090,  // 64-port ADSL AnnexB Board
    ADBF2 =     131084,  // 64-port ADSL AnnexB Board
    SHEA =      131111,  // 32-port SHDSL Board
    //MA5300
    EADB =      217,     // 48-port ADSL AnnexA Board
    EADJ =      802,     // 48-port ADSL AnnexB Board
    ESHA =      215      // 24-port SHDSL Board
};

inline BoardType::Enum toBoardType(int slotType) {
    switch (slotType) {
    case ADEF:
    case ADEF2:
    case EADB:
        return BoardType::AnnexA;
    case ADBF:
    case ADBF2:
    case EADJ:
        return BoardType::AnnexB;
    case SHEA:
    case ESHA:
        return BoardType::Shdsl;
    default:
        return BoardType::Other;
    }
}
}

namespace DeviceModel
{
enum Enum {
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
                                    "MXA-32", "MXA-64", "LTE-8ST", "LTP-8X"
                                  };

inline DeviceModel::Enum from(QString deviceModel)
{
    if (deviceModel == DeviceModelName[DeviceModel::DES3526]) {
        return DeviceModel::DES3526;
    } else if (deviceModel == DeviceModelName[DeviceModel::MA5600]) {
        return DeviceModel::MA5600;
    } else if (deviceModel == DeviceModelName[DeviceModel::LTP8X]) {
        return DeviceModel::LTP8X;
    } else if (deviceModel == DeviceModelName[DeviceModel::LTE8ST]) {
        return DeviceModel::LTE8ST;
    } else if (deviceModel == DeviceModelName[DeviceModel::DES3528]) {
        return DeviceModel::DES3528;
    } else if (deviceModel == DeviceModelName[DeviceModel::MA5300]) {
        return DeviceModel::MA5300;
    } else if (deviceModel == DeviceModelName[DeviceModel::MXA64]) {
        return DeviceModel::MXA64;
    } else if (deviceModel == DeviceModelName[DeviceModel::DES3552]) {
        return DeviceModel::DES3552;
    } else if (deviceModel == DeviceModelName[DeviceModel::MXA32]) {
        return DeviceModel::MXA32;
    } else if (deviceModel == DeviceModelName[DeviceModel::MA5616]) {
        return DeviceModel::MA5616;
    } else if (deviceModel == DeviceModelName[DeviceModel::DES3550]) {
        return DeviceModel::DES3550;
    } else {
        return DeviceModel::Other;
    }
}

inline QString toString(DeviceModel::Enum deviceModel)
{
    return DeviceModelName[deviceModel];
}
}

namespace DeviceType
{
enum Enum {
    Other =     0,
    Switch =    1,
    Dslam =     2,
    Olt =       3
};

const QString DeviceTypeName[] = { "Other", "Switch", "Dslam", "Olt" };

inline DeviceType::Enum from(QString deviceType)
{
    if (deviceType == DeviceTypeName[DeviceType::Switch]) {
        return DeviceType::Switch;
    } else if (deviceType == DeviceTypeName[DeviceType::Dslam]) {
        return DeviceType::Dslam;
    } else if (deviceType == DeviceTypeName[DeviceType::Olt]) {
        return DeviceType::Olt;
    } else {
        return DeviceType::Other;
    }
}

inline DeviceType::Enum from(DeviceModel::Enum deviceModel)
{
    switch (deviceModel) {
    case DeviceModel::DES3526:
    case DeviceModel::DES3528:
    case DeviceModel::DES3550:
    case DeviceModel::DES3552:
        return DeviceType::Switch;
    case DeviceModel::MA5600:
    case DeviceModel::MA5300:
    case DeviceModel::MA5616:
    case DeviceModel::MXA32:
    case DeviceModel::MXA64:
        return DeviceType::Dslam;
    case DeviceModel::LTP8X:
    case DeviceModel::LTE8ST:
        return DeviceType::Olt;
    case DeviceModel::Other:
    case DeviceModel::Count:
        return DeviceType::Other;
    default:
        return DeviceType::Other;
    }
}

inline QString toString(DeviceType::Enum deviceType)
{
    return DeviceTypeName[deviceType];
}
}

namespace DslPortState
{
enum Enum {
    Other =         0,
    Up =            1,
    Down =          2,
    Activating =    3,
    Defective =     4
};

const QString DslPortStateName[] = {"Other", "Up", "Down", "Activating",
                                    "Defective" };

inline DslPortState::Enum from(QString dslPortState)
{
    if (dslPortState == DslPortStateName[DslPortState::Activating]) {
        return DslPortState::Activating;
    } else if (dslPortState == DslPortStateName[DslPortState::Up]) {
        return DslPortState::Up;
    } else if (dslPortState == DslPortStateName[DslPortState::Down]) {
        return DslPortState::Down;
    } else if (dslPortState == DslPortStateName[DslPortState::Defective]) {
        return DslPortState::Defective;
    } else {
        return DslPortState::Other;
    }
}

inline DslPortState::Enum from(long snmpValue)
{
    switch (snmpValue) {
    case 1:
        return DslPortState::Up;
    case 2:
    case 6:
        return DslPortState::Down;
    case 5:
    case 65536:
        return DslPortState::Activating;
    case 65548:
        return DslPortState::Defective;
    default:
        return DslPortState::Other;
    }
}

inline DslPortState::Enum from(DslPortState::Enum operStatus,
                               DslPortState::Enum adminStatus)
{
    if (adminStatus == DslPortState::Down)
        return DslPortState::Down;
    else if ((adminStatus == DslPortState::Up)
             && (operStatus == DslPortState::Up))
        return DslPortState::Up;
    else if ((adminStatus == DslPortState::Up)
             && (operStatus == DslPortState::Down))
        return DslPortState::Activating;
    else
        return DslPortState::Other;
}

inline QString toString(DslPortState::Enum dslPortState)
{
    return DslPortStateName[dslPortState];
}
}

namespace OntType
{
enum Enum {
    Nte = 1,
    Ntp = 2
};
}

namespace NteModel
{
enum Enum {
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

namespace NteState
{
enum Enum {
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

namespace NtpState
{
enum Enum {
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
enum Enum {
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
enum Enum {
    ReadSession,
    WriteSession
};
}

namespace SwitchPortState
{
enum Enum {
    Other =         0,
    Up =            1,
    Down =          2
};

const QString SwitchPortStateName[] = { "Other", "Up", "Down" };

inline SwitchPortState::Enum from(QString switchPortState)
{
    if (switchPortState == SwitchPortStateName[SwitchPortState::Up]) {
        return SwitchPortState::Up;
    } else if (switchPortState == SwitchPortStateName[SwitchPortState::Down]) {
        return SwitchPortState::Down;
    } else {
        return SwitchPortState::Other;
    }
}

inline SwitchPortState::Enum from(long snmpValue)
{
    switch (snmpValue) {
    case 1:
        return SwitchPortState::Up;
    case 2:
        return SwitchPortState::Down;
    default:
        return SwitchPortState::Other;
    }
}

inline QString toString(SwitchPortState::Enum state)
{
    return SwitchPortStateName[state];
}
}

namespace VlanState
{
enum Enum {
    Untag = 0,
    Tag =   1,
    None =  2
};

const QString VlanStateName[] = { "Untag", "Tag", "None" };

inline VlanState::Enum from(QString vlanState)
{
    if (vlanState == VlanStateName[VlanState::Untag]) {
        return VlanState::Untag;
    } else if (vlanState == VlanStateName[VlanState::Tag]) {
        return VlanState::Tag;
    } else {
        return VlanState::None;
    }
}

inline QString toString(VlanState::Enum state)
{
    return VlanStateName[state];
}
}

#endif // CUSTOMTYPES_H
