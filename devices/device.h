#ifndef DEVICE_H
#define DEVICE_H

#include <customtypes.h>
#include <snmpclient.h>

class Device : public QObject
{
    Q_OBJECT
public:
    Device(QObject *parent = 0);
    Device(QString name, QString ip, QObject *parent = 0);
    Device(Device *source, QObject *parent = 0);

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

    typedef QSharedPointer<Device> Ptr;
signals:
    void modified();
private:
    QString mIp;
    QString mName;
protected:
    QString mError;
};



#endif // DEVICE_H
