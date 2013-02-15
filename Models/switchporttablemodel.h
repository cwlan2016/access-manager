#ifndef SWITCHPORTTABLEMODEL_H
#define SWITCHPORTTABLEMODEL_H

#ifdef _MSC_VER
#include "../customtypes.h"
#include "../snmpclient.h"
#include "../Info/switchinfo.h"
#include "../Info/switchportinfo.h"
#else
#include "customtypes.h"
#include "snmpclient.h"
#include "Info/switchinfo.h"
#include "Info/switchportinfo.h"
#endif

class SwitchPortTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SwitchPortTableModel(SwitchInfo::Ptr parentDevice,
                                  QObject *parent = 0);

    int rowCount(const QModelIndex &parentDevice) const;
    int columnCount(const QModelIndex &parentDevice) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void sort(int column, Qt::SortOrder order);

    bool memberMulticastVlan(int port);
    bool setMemberMulticastVlan(int port, bool value);

    VlanState::Enum memberInetVlan(int port);
    VlanState::Enum memberIptvVlan(int port);

    bool setMemberInternetService(int port);
    bool setMemberInternetWithIptvStbService(int port);

    bool updateInfoPort(int indexPort);
    bool updateInfoAllPort();

    bool getVlanSettings();

    QString error() const;

private:
    void createList();

    bool updateInfoPort(QScopedPointer<SnmpClient> &snmpClient,
                        SwitchPortInfo::Ptr updatingPort);

    bool getUnicastVlanSettings(OidPair oidVlan,
                                QBitArray &vlanPortArray, QString vlanName);
    bool getMulticastVlanSettings();

    bool sendVlanSetting(QVector<OidPair> &oidStringList,
                         QList<QBitArray> &arrayList, bool ismv);

    QString mError;
    int mIptvMultVlanTag;
    SwitchInfo::Ptr mParentDevice;
    QBitArray mInetVlanAllMember;
    QBitArray mInetVlanUntagMember;
    QBitArray mMulticastVlanMember;
    QBitArray mIptvUnicastVlanAllMember;
    QBitArray mIptvUnicastVlanUntagMember;
    QVector<SwitchPortInfo::Ptr> mList;
};

#endif // SWITCHPORTABLEMODEL_H
