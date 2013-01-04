#include "customtypes.h"

namespace BoardType
{
BoardType::Enum fromString(QString boardType)
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
DeviceModel::Enum fromString(QString deviceModel)
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
DeviceType::Enum fromString(QString deviceType)
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

QString toString(DeviceType::Enum deviceType)
{
    return DeviceTypeName[deviceType];
}
}
