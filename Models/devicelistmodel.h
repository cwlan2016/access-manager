#ifndef DEVICELISTMODEL_H
#define DEVICELISTMODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtCore/QXmlStreamWriter>
#ifdef _MSC_VER
#include "../Info/switchinfo.h"
#include "../Info/dslaminfo.h"
#include "../Info/oltinfo.h"
#else
#include "Info/switchinfo.h"
#include "Info/dslaminfo.h"
#include "Info/oltinfo.h"
#endif

class DeviceListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit DeviceListModel(QObject *parentDevice = 0);
    int rowCount(const QModelIndex &parentDevice) const;
    int columnCount(const QModelIndex &parentDevice) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool insertRow(int row, const QModelIndex &parentDevice);
    bool removeRow(int row, const QModelIndex &parentDevice);
    std::vector<DeviceInfo::Ptr> &deviceList();
    void setModified(bool state);
    BoardListModel *boardListModel(QModelIndex index);
    //file operations
    bool load();
    bool save();
    bool isModified();
    //vlans
    int inetVlan(QModelIndex index);
    int iptvVlan(QModelIndex index);
    //olt profiles
    QStringListModel *serviceProfileOltListModel(QModelIndex index);
    QStringListModel *multicastProfileOltListModel(QModelIndex index);
    //Get specific info from device
    bool getVlanTagFromDevice(QModelIndex index);
    bool getBoardListFromDevice(QModelIndex index);
    bool getProfilesFromDevice(QModelIndex index);
    //error
    QString error() const;
private:
    std::vector<DeviceInfo::Ptr> mDeviceList;
    QString mError;
    QString mDeviceListPath;
    QString mDeviceListBackupPath;
    bool mModified;
    bool exist();
    bool backup();

    void createSwitchElement(QXmlStreamWriter &writer, const SwitchInfo::Ptr &deviceInfo);
    void createDslamElement(QXmlStreamWriter &writer, const DslamInfo::Ptr &deviceInfo);
    void createOltElement(QXmlStreamWriter &writer, const OltInfo::Ptr &deviceInfo);
    void createOltProfileList(QXmlStreamWriter &writer, const OltProfileMap &profileMap,
                              QString typeElem);
};

#endif // DEVICELISTMODEL_H
