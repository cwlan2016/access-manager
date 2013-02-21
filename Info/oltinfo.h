#ifndef OLTINFO_H
#define OLTINFO_H

#include "deviceinfo.h"

class OltInfo : public DeviceInfo
{
    Q_OBJECT
public:
    OltInfo(QObject *parent = 0);
    OltInfo(QString name, QString ip, QObject *parent = 0);

    QString serviceProfile(int index);
    void addServiceProfile(int index, QString profileName);

    QString multicastProfile(int index);
    void addMulticastProfile(int index, QString profileName);

    OltProfileMap &serviceProfileList();
    QStringListModel *serviceProfileListModel(QObject *parent = 0);

    OltProfileMap &multicastProfileList();
    QStringListModel *multicastProfileListModel(QObject *parent = 0);

    DeviceType::Enum deviceType() const;

    bool getServiceDataFromDevice();

    typedef QSharedPointer<OltInfo> Ptr;

private:
    bool getProfileList(OltProfileMap &profileList, OidPair oid);
    QStringListModel *createStringListModel(OltProfileMap &profileList,
                                                   QObject *parent);

    OltProfileMap mServiceProfileList;
    OltProfileMap mMulticastProfileList;
};



#endif // OLTINFO_H
