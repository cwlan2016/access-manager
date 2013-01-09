#ifndef SWITCHPORTLISTMODEL_H
#define SWITCHPORTLISTMODEL_H

#include "stdafx.h"
#include "Info/switchportinfo.h"
#include "snmpclient.h"
#include "converters.h"
#include "basicdialogs.h"
#include "customtypes.h"
#include "customsnmpfunctions.h"

class SwitchPortListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SwitchPortListModel(DeviceModel deviceModel, QObject* parent = 0);
    int rowCount(const QModelIndex& parentDevice) const;
    int columnCount(const QModelIndex& parentDevice) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void sort(int column, Qt::SortOrder order);
    Qt::ItemFlags flags(const QModelIndex& index) const;
    //инициализация
    DeviceModel deviceModel();
    void setIP(QString ip);
    void setInetVlanTag(uint vlanTag);
    void setIptvVlanTag(uint vlanTag);
    QString ip();
    //ошибки
    QString error() const;
    //информация по порту
    bool memberMulticastVlan(int port);
    VlanState memberInetVlan(int port);
    VlanState memberIptvVlan(int port);
    //настройка порта
    bool setMemberMulticastVlan(int port, bool value);
    bool setMemberInternetService(int port);
    bool setMemberInternetWithIptvStbService(int port);
    //обновление информации
    bool updateInfoPort(int indexPort);
    bool updateInfoAllPort();
    bool getVlanSettings();
private:
    std::vector<SwitchPortInfo::Ptr> mPortList;
    QBitArray mMulticastVlanMember;
    QBitArray mIptvUnicastVlanAllMember;
    QBitArray mIptvUnicastVlanUntagMember;
    QBitArray mInetVlanAllMember;
    QBitArray mInetVlanUntagMember;
    QString mError;
    QString mIp;
    DeviceModel mDeviceModel;
    uint mInetVlanTag;
    uint mIptvVlanTag;
    uint mIptvMultVlanTag;
    void createList();
    bool getUnicastVlanSettings(const oid *oidVlan, int oidVlanLen, QBitArray &vlanPortArray, QString vlanName);
    bool getMulticastVlanSettings();
    bool sendVlanSetting(QVector<OidPair> &oidStringList, QList<QBitArray> &arrayList, bool ismv);
};

#endif // SWITCHPORTLISTMODEL_H
