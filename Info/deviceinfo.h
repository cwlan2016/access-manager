#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include "stdafx.h"
#include "converters.h"
#include "customtypes.h"

class DeviceInfo
{
public:
    DeviceInfo();
    DeviceInfo(QString name, QString ip, DeviceModel deviceModel);
    virtual ~DeviceInfo();
    QString name() const;
    QString ip() const;
    DeviceModel deviceModel() const;
    DeviceType deviceType() const;
    QString error() const;
    void setName(const QString name);
    void setIP(const QString ip);
    void setDeviceModel(const DeviceModel deviceModel);
    void setDeviceType(const DeviceType deviceType);
    virtual bool getServiceDataFromDevice();

    typedef std::shared_ptr<DeviceInfo> Ptr;
protected:
    QString mName;
    QString mIp;
    DeviceType mDeviceType;
    DeviceModel mDeviceModel;
    QString mError;
};



#endif // DEVICEINFO_H
