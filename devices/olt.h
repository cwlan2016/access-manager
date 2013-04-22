#ifndef OLT_H
#define OLT_H

#include "device.h"

class Olt : public Device
{
    Q_OBJECT
public:
    Olt(QObject *parent = 0);
    Olt(QString name, QString ip, QObject *parent = 0);
    Olt(Device *source, QObject *parent = 0);

    QString serviceProfile(int index) const;
    void addServiceProfile(int index, QString profileName);

    QString multicastProfile(int index) const;
    void addMulticastProfile(int index, QString profileName);

    OltProfileMap &serviceProfileList();
    QStringListModel *serviceProfileListModel(QObject *parent = 0);

    OltProfileMap &multicastProfileList();
    QStringListModel *multicastProfileListModel(QObject *parent = 0);

    DeviceType::Enum deviceType() const;

    bool getServiceDataFromDevice();

    typedef QSharedPointer<Olt> Ptr;

private:
    bool getProfileList(OltProfileMap &profileList, OidPair oid);
    QStringListModel *createStringListModel(OltProfileMap &profileList,
                                                   QObject *parent);

    OltProfileMap mServiceProfileList;
    OltProfileMap mMulticastProfileList;
};



#endif // OLT_H
