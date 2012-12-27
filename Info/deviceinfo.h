#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <memory>

#ifdef _MSC_VER
#include "../customtypes.h"
#else
#include "customtypes.h"
#endif

class DeviceInfo
{
public:
    DeviceInfo();
    DeviceInfo(QString name, QString ip, DeviceModel::Enum deviceModel);
    virtual ~DeviceInfo();
    QString name() const;
    QString ip() const;
    DeviceModel::Enum deviceModel() const;
    DeviceType::Enum deviceType() const;
    QString error() const;
    void setName(const QString name);
    void setIP(const QString ip);
    void setDeviceModel(const DeviceModel::Enum deviceModel);
    void setDeviceType(const DeviceType::Enum deviceType);
    virtual bool getServiceDataFromDevice();

    typedef std::shared_ptr<DeviceInfo> Ptr;
protected:
    QString mName;
    QString mIp;
    DeviceType::Enum mDeviceType;
    DeviceModel::Enum mDeviceModel;
    QString mError;
};



#endif // DEVICEINFO_H
