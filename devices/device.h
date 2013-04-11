#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#ifdef _MSC_VER
#include "../customtypes.h"
#include "../snmpclient.h"
#else
#include "customtypes.h"
#include "snmpclient.h"
#endif

class DeviceInfo : public QObject
{
    Q_OBJECT
public:
    DeviceInfo(QObject *parent = 0);
    DeviceInfo(QString name, QString ip, QObject *parent = 0);
    DeviceInfo(DeviceInfo *source, QObject *parent = 0);

    QString name() const;
    void setName(const QString name);

    QString ip() const;
    void setIp(const QString ip);

    virtual DeviceModel::Enum deviceModel() const;
    virtual DeviceType::Enum deviceType() const;

    QString error() const;

    virtual bool getServiceDataFromDevice();

    virtual void fillPdu(SnmpClient::Ptr snmpClient, int portIndex = -1);
    virtual void parsePdu(SnmpClient::Ptr snmpClient);

    typedef QSharedPointer<DeviceInfo> Ptr;

protected:
    QString mIp;
    QString mName;
    QString mError;
};



#endif // DEVICEINFO_H
