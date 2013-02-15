#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#ifdef _MSC_VER
#include "../customtypes.h"
#else
#include "customtypes.h"
#endif

class DeviceInfo : public QObject
{
    Q_OBJECT
public:
    DeviceInfo(QObject *parent = 0);
    DeviceInfo(QString name, QString ip, DeviceModel::Enum deviceModel,
               QObject *parent = 0);

    QString name() const;
    void setName(const QString name);

    QString ip() const;
    void setIp(const QString ip);

    DeviceModel::Enum deviceModel() const;
    void setDeviceModel(const DeviceModel::Enum deviceModel);

    DeviceType::Enum deviceType() const;
    void setDeviceType(const DeviceType::Enum deviceType);

    QString error() const;

    virtual bool getServiceDataFromDevice();

    typedef QSharedPointer<DeviceInfo> Ptr;

protected:
    QString mIp;
    QString mName;
    QString mError;
    DeviceType::Enum mDeviceType;
    DeviceModel::Enum mDeviceModel;
};



#endif // DEVICEINFO_H
