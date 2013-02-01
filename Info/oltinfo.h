#ifndef OLTINFO_H
#define OLTINFO_H

#include <QtCore/QStringListModel>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include "deviceinfo.h"

class OltInfo : public DeviceInfo
{
    Q_OBJECT
public:
    OltInfo(QObject *parent = 0);
    OltInfo(QString name, QString ip, DeviceModel::Enum deviceModel, QObject *parent = 0);
    QString serviceProfile(int index);
    QString multicastProfile(int index);
    void addServiceProfile(int index, QString profileName);
    void addMulticastProfile(int index, QString profileName);
    bool getServiceDataFromDevice();
    OltProfileMap &serviceProfileList();
    OltProfileMap &multicastProfileList();
    QStringListModel *serviceProfileListModel(QObject *parent = 0);
    QStringListModel *multicastProfileListModel(QObject *parent = 0);

    typedef QSharedPointer<OltInfo> Ptr;
private:
    OltProfileMap mServiceProfileList;
    OltProfileMap mMulticastProfileList;
    bool getProfileList(OltProfileMap &profileList, const oid *oidProfileName, int oidLen);
    QStringListModel *createStringListModelFromMap(OltProfileMap &profileList, QObject *parent);
};



#endif // OLTINFO_H
