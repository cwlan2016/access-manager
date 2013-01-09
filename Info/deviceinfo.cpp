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

DeviceInfo::DeviceInfo(QString name, QString ip, DeviceModel::Enum deviceModel, QObject *parent) :
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

QString DeviceInfo::ip() const
{
    return mIp;
}

DeviceModel::Enum DeviceInfo::deviceModel() const
{
    return mDeviceModel;
}

DeviceType::Enum DeviceInfo::deviceType() const
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

void DeviceInfo::setDeviceModel(const DeviceModel::Enum deviceModel)
{
    mDeviceModel = deviceModel;
    mDeviceType = DeviceType::from(mDeviceModel);
}

void DeviceInfo::setDeviceType(const DeviceType::Enum deviceType)
{
    mDeviceType = deviceType;
}

bool DeviceInfo::getServiceDataFromDevice()
{
    return false;
}
