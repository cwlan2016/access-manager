#ifndef SWITCHINFO_H
#define SWITCHINFO_H

#include "deviceinfo.h"

class SwitchInfo : public DeviceInfo
{
public:
    SwitchInfo();
    SwitchInfo(QString name, QString ip, DeviceModel::Enum deviceModel);
    int inetVlanTag() const;
    int iptvVlanTag() const;
    void setInetVlanTag(int vlanTag);
    void setIptvVlanTag(int vlanTag);
    virtual bool getServiceDataFromDevice();
    bool saveConfig();
    typedef std::shared_ptr<SwitchInfo> Ptr;
private:
    int mInetVlanTag;
    int mIptvVlanTag;
};



#endif // SWITCHINFO_H
