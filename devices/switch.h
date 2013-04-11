#ifndef SWITCHINFO_H
#define SWITCHINFO_H

#include "deviceinfo.h"

class SwitchInfo : public DeviceInfo
{
    Q_OBJECT
public:
    explicit SwitchInfo(QObject *parent = 0);
    SwitchInfo(QString name, QString ip, QObject *parent = 0);
    SwitchInfo(DeviceInfo *source, QObject *parent = 0);

    int inetVlanTag() const;
    void setInetVlanTag(int vlanTag);

    int iptvVlanTag() const;
    void setIptvVlanTag(int vlanTag);

    DeviceType::Enum deviceType() const;
    virtual int countPorts();

    bool getServiceDataFromDevice();

    bool saveConfig();

    typedef QSharedPointer<SwitchInfo> Ptr;

private:
    int mInetVlanTag;
    int mIptvVlanTag;
};

#endif // SWITCHINFO_H
