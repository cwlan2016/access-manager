#include "device.h"

Device::Device(QObject *parent) :
    QObject(parent)
{
}

Device::Device(QString name, QString ip, QObject *parent) :
    QObject(parent)
{
    mName = name;
    mIp = ip;
}

Device::Device(Device *source, QObject *parent) :
    QObject(parent)
{
    mName = source->name();
    mIp = source->ip();
}

QString Device::name() const
{
    return mName;
}

void Device::setName(const QString name)
{
    if (mName.compare(name) != 0) {
        mName = name;
        emit modified();
    }
}

QString Device::ip() const
{
    return mIp;
}

void Device::setIp(const QString ip)
{
    if (mIp.compare(ip) != 0) {
        mIp = ip;
        emit modified();
    }
}

DeviceModel::Enum Device::deviceModel() const
{
    return DeviceModel::Other;
}

DeviceType::Enum Device::deviceType() const
{
    return DeviceType::Other;
}

QString Device::error() const
{
    return mError;
}

bool Device::getServiceDataFromDevice()
{
    return false;
}

//QString Device::id() const
//{
//    return deviceModel() + ip();
//}

//QHash<int, QByteArray> Device::roleNames() const
//{
//    QHash<int, QByteArray> names;
//    names[NameRole] = "name";
//    names[IpRole] = "ip";
//    names[TypeRole] = "type";
//    names[ModelRole] = "model";

//    return names;
//}

QVariant Device::data(int role) const
{
    switch(role) {
    case NameRole:
        return name();
    case IpRole:
        return ip();
    case TypeRole:
        return deviceType();
    case ModelRole:
        return deviceModel();
    default:
        return QVariant();
    }
}

void Device::fillPdu(SnmpClient::Ptr snmpClient, int portIndex)
{
    Q_UNUSED(snmpClient)
    Q_UNUSED(portIndex)
}

void Device::parsePdu(SnmpClient::Ptr snmpClient)
{
    Q_UNUSED(snmpClient)
}
