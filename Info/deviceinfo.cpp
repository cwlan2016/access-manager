#include "deviceinfo.h"

#ifdef _MSC_VER
#include "../converters.h"
#else
#include "converters.h"
#endif

DeviceInfo::DeviceInfo(QObject *parent) :
    QObject(parent)
{
    setDeviceModel(DeviceModel::Other);
}

DeviceInfo::DeviceInfo(QString name, QString ip, DeviceModel::Enum deviceModel,
                       QObject *parent) :
    QObject(parent)
{
    mName = name;
    mIp = ip;
    setDeviceModel(deviceModel);
}

QString DeviceInfo::name() const
{
    return mName;
}

void DeviceInfo::setName(const QString name)
{
    mName = name;
}

QString DeviceInfo::ip() const
{
    return mIp;
}

void DeviceInfo::setIp(const QString ip)
{
    mIp = ip;
}

DeviceModel::Enum DeviceInfo::deviceModel() const
{
    return mDeviceModel;
}

void DeviceInfo::setDeviceModel(const DeviceModel::Enum deviceModel)
{
    mDeviceModel = deviceModel;
    mDeviceType = DeviceType::from(mDeviceModel);
}

DeviceType::Enum DeviceInfo::deviceType() const
{
    return mDeviceType;
}

void DeviceInfo::setDeviceType(const DeviceType::Enum deviceType)
{
    mDeviceType = deviceType;
}

QString DeviceInfo::error() const
{
    return mError;
}

bool DeviceInfo::getServiceDataFromDevice()
{
    return false;
}
