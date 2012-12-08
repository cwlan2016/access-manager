#ifndef CUSTOMTYPES_H
#define CUSTOMTYPES_H

#include "stdafx.h"

typedef std::unordered_map<int, QString> OltProfileMap;
//Enums
enum class PageType
{
    DeviceListPage,
    DslamPage,
    EditDslamPage,
    SwitchPage,
    OltPage,
    AboutPage,
    SettingsPage
};

enum class DeviceType : short
{
    Other =     0,
    Switch =    1,
    Dslam =     2,
    Olt =       3
};

enum class DeviceModel : short
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
    LTP8X =     11
};

enum class BoardType : short
{
    AnnexA =    0,
    AnnexB =    1,
    Shdsl =     2,
    Other =     3
};

enum class SessionType
{
    ReadSession,
    WriteSession
};

enum class VlanState : short
{
    Untag = 0,
    Tag =   1,
    None =  2
};
// Первые два индекса не менять, это snmp значения
enum class DslPortState : short
{
    Up =            1,
    Down =          2,
    Activating =    3,
    Defective =     4,
    Other =         5
};

enum class LtpOntState : short
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

enum class LteOntState : short
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

enum class OntType : short
{
    Nte = 1,
    Ntp = 2
};

enum class NteModel : short
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


#endif // CUSTOMTYPES_H
