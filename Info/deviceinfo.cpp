#include "deviceinfo.h"

DeviceInfo::DeviceInfo()
{
    setDeviceModel(DeviceModel::Other);
}

DeviceInfo::DeviceInfo(QString name, QString ip, DeviceModel deviceModel)
{
    mName = name;
    mIp = ip;
    setDeviceModel(deviceModel);
}

DeviceInfo::~DeviceInfo()
{
}

QString DeviceInfo::name() const
{
    return mName;
}

QString DeviceInfo::ip() const
{
    return mIp;
}

DeviceModel DeviceInfo::deviceModel() const
{
    return mDeviceModel;
}

DeviceType DeviceInfo::deviceType() const
{
    return mDeviceType;
}

QString DeviceInfo::error() const
{
    return mError;
}

void DeviceInfo::setName(const QString name)
{
    mName = name;
}

void DeviceInfo::setIP(const QString ip)
{
    mIp = ip;
}

void DeviceInfo::setDeviceModel(const DeviceModel deviceModel)
{
    mDeviceModel = deviceModel;
    mDeviceType = DeviceTypeFromDeviceModel(mDeviceModel);
}

void DeviceInfo::setDeviceType(const DeviceType deviceType)
{
    mDeviceType = deviceType;
}

bool DeviceInfo::getServiceDataFromDevice()
{
    return false;
}
