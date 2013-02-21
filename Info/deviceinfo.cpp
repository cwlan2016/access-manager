#include "deviceinfo.h"

#ifdef _MSC_VER
#include "../converters.h"
#else
#include "converters.h"
#endif

DeviceInfo::DeviceInfo(QObject *parent) :
    QObject(parent)
{
    //setDeviceModel(DeviceModel::Other);
}

DeviceInfo::DeviceInfo(QString name, QString ip, QObject *parent) :
    QObject(parent)
{
    mName = name;
    mIp = ip;
    //setDeviceModel(deviceModel);
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
    return DeviceModel::Other;
}

DeviceType::Enum DeviceInfo::deviceType() const
{
    return DeviceType::Other;
}

QString DeviceInfo::error() const
{
    return mError;
}

bool DeviceInfo::getServiceDataFromDevice()
{
    return false;
}

void DeviceInfo::fillPdu(SnmpClient::Ptr snmpClient, int portIndex)
{
    Q_UNUSED(snmpClient)
    Q_UNUSED(portIndex)
}

void DeviceInfo::parsePdu(SnmpClient::Ptr snmpClient)
{
    Q_UNUSED(snmpClient)
}
