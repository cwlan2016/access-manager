#ifndef DEVICELISTMODEL_H
#define DEVICELISTMODEL_H

#include "stdafx.h"
#include "config.h"
#include "Info/switchinfo.h"
#include "Info/dslaminfo.h"
#include "Info/boardinfo.h"
#include "constant.h"
#include "basicdialogs.h"
#include "snmpclient.h"
#include "devicelisthandler.h"

class DeviceListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit DeviceListModel(QObject* parentDevice = 0);
    int rowCount(const QModelIndex& parentDevice) const;
    int columnCount(const QModelIndex& parentDevice) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    bool insertRow(int row, const QModelIndex& parentDevice);
    bool removeRow(int row, const QModelIndex& parentDevice);
    std::vector<DeviceInfo::Ptr>& deviceList();
    void setModified(bool state);
    BoardListModel* boardListModel(QModelIndex index);
    //файловые операции
    bool load();
    bool save();
    void toDefault();
    bool isModified();
    //вланы
    int inetVlan(QModelIndex index);
    int iptvVlan(QModelIndex index);
    bool getVlanTagFromDevice(QModelIndex index);
    bool getBoardListFromDevice(QModelIndex index);
    //ошибки
    QString error() const;
private:
    std::vector<DeviceInfo::Ptr> mDeviceList;
    QString mError;
    QString mDeviceListPath;
    QString mDeviceListBackupPath;
    bool mModified;
    bool exist();
    bool backup();

    void createSwitchElement(QXmlStreamWriter& writer, const SwitchInfo::Ptr& deviceInfo);
    void createDslamElement(QXmlStreamWriter& writer, const DslamInfo::Ptr& deviceInfo);
    void createOltElement(QXmlStreamWriter& writer, const OltInfo::Ptr& deviceInfo);
    void createOltProfileList(QXmlStreamWriter& writer, const OltProfileMap& profileMap, QString typeElem);
};

#endif // DEVICELISTMODEL_H
