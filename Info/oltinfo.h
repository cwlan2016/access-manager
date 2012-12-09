#ifndef OLTINFO_H
#define OLTINFO_H

#include "stdafx.h"
#include "snmpclient.h"
#include "deviceinfo.h"
#include "constant.h"

class OltInfo : public DeviceInfo
{
public:
    OltInfo();
    OltInfo(QString name, QString ip, DeviceModel deviceModel);
    QString serviceProfile(int index);
    QString multicastProfile(int index);
    void addServiceProfile(int index, QString profileName);
    void addMulticastProfile(int index, QString profileName);
    virtual bool getServiceDataFromDevice();
    OltProfileMap& serviceProfileList();
    OltProfileMap& multicastProfileList();
    QStringListModel* serviceProfileListModel(QObject* parent = 0);
    QStringListModel* multicastProfileListModel(QObject* parent = 0);

    typedef std::shared_ptr<OltInfo> Ptr;
private:
    OltProfileMap mServiceProfileList;
    OltProfileMap mMulticastProfileList;
    bool getProfileList(OltProfileMap& profileList, const oid* oidProfileName, int oidLen);
    QStringListModel* createStringListModelFromMap(OltProfileMap& profileList, QObject* parent);
};



#endif // OLTINFO_H
