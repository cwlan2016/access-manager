#ifndef SWITCHPORTLISTMODEL_H
#define SWITCHPORTLISTMODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtCore/QBitArray>
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

class SwitchPortListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SwitchPortListModel(SwitchInfo::Ptr parentDevice, QObject *parent = 0);
    int rowCount(const QModelIndex &parentDevice) const;
    int columnCount(const QModelIndex &parentDevice) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void sort(int column, Qt::SortOrder order);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    //инициализация
    QString error() const;
    //информация по порту
    bool memberMulticastVlan(int port);
    VlanState::Enum memberInetVlan(int port);
    VlanState::Enum memberIptvVlan(int port);
    //настройка порта
    bool setMemberMulticastVlan(int port, bool value);
    bool setMemberInternetService(int port);
    bool setMemberInternetWithIptvStbService(int port);
    //обновление информации
    bool updateInfoPort(int indexPort);
    bool updateInfoAllPort();
    bool getVlanSettings();
private:
    bool updateInfoPort(QScopedPointer<SnmpClient> &snmpClient, SwitchPortInfo::Ptr updatingPort);
    void createList();
    bool getUnicastVlanSettings(const oid *oidVlan, int oidVlanLen, QBitArray &vlanPortArray, QString vlanName);
    bool getMulticastVlanSettings();
    bool sendVlanSetting(QVector<OidPair> &oidStringList, QList<QBitArray> &arrayList, bool ismv);

    QString mError;
    QBitArray mInetVlanAllMember;
    QBitArray mInetVlanUntagMember;
    int mIptvMultVlanTag;
    QBitArray mIptvUnicastVlanAllMember;
    QBitArray mIptvUnicastVlanUntagMember;
    QVector<SwitchPortInfo::Ptr> mList;
    QBitArray mMulticastVlanMember;
    SwitchInfo::Ptr mParentDevice;
};

#endif // SWITCHPORTLISTMODEL_H
