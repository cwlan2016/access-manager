#ifndef SWITCHINFO_H
#define SWITCHINFO_H

#include "deviceinfo.h"

class SwitchInfo : public DeviceInfo
{
    Q_OBJECT
public:
    SwitchInfo(QObject *parent = 0);
    SwitchInfo(QString name, QString ip, DeviceModel::Enum deviceModel,
               QObject *parent = 0);

    int inetVlanTag() const;
    void setInetVlanTag(int vlanTag);

    int iptvVlanTag() const;
    void setIptvVlanTag(int vlanTag);

    bool getServiceDataFromDevice();

    bool saveConfig();

    typedef QSharedPointer<SwitchInfo> Ptr;

private:
    int mInetVlanTag;
    int mIptvVlanTag;
};

#endif // SWITCHINFO_H
