#ifndef SWITCH_H
#define SWITCH_H

#include "device.h"
#include <ports/switchport.h>

class Switch : public Device
{
    Q_OBJECT
public:
    explicit Switch(QObject *parent = 0);
    Switch(QString name, QString ip, QObject *parent = 0);
    Switch(Device *source, QObject *parent = 0);

    int inetVlanTag() const;
    void setInetVlanTag(int vlanTag);

    int iptvVlanTag() const;
    void setIptvVlanTag(int vlanTag);

    DeviceType::Enum deviceType() const;
    virtual int countPorts();
    virtual SwitchPort::Ptr createPort(QObject *parentModel);

    bool getServiceDataFromDevice();

    bool saveConfig();

    typedef QSharedPointer<Switch> Ptr;

private:
    int mInetVlanTag;
    int mIptvVlanTag;
};

#endif // SWITCH_H
