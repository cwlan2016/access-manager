#include "customtypes.h"

namespace BoardType
{
BoardType::Enum from(QString boardType)
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

QString toString(BoardType::Enum boardType)
{
    return BoardTypeName[boardType];
}
}

namespace DeviceModel
{
DeviceModel::Enum from(QString deviceModel)
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

QString toString(DeviceModel::Enum deviceModel)
{
    return DeviceModelName[deviceModel];
}
}

namespace DeviceType
{
DeviceType::Enum from(QString deviceType)
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

DeviceType::Enum from(DeviceModel::Enum deviceModel)
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
    }

    return DeviceType::Other;
}

QString toString(DeviceType::Enum deviceType)
{
    return DeviceTypeName[deviceType];
}
}

namespace DslPortState
{
DslPortState::Enum from(QString dslPortState)
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

QString toString(DslPortState::Enum dslPortState)
{
    return DslPortStateName[dslPortState];
}
}
