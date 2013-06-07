#ifndef DEVICETABLEMODEL_H
#define DEVICETABLEMODEL_H

#include <devices/dslam.h>
#include <devices/olt.h>
#include <devices/switch.h>

class DeviceTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit DeviceTableModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool insertRow(int row, const QModelIndex &parent);
    bool removeRow(int row, const QModelIndex &parent);
    bool isModified();

    bool load();
    bool save();

    QVector<Device::Ptr> &deviceList();

    BoardTableModel *boardListModel(QModelIndex index);

    int inetVlan(QModelIndex index);
    int iptvVlan(QModelIndex index);

    QStringListModel *serviceProfileOltListModel(QModelIndex index);
    QStringListModel *multicastProfileOltListModel(QModelIndex index);

    bool getServiceDataFromDevice(QModelIndex index);

    QString error() const;

private:
    bool exist();
    bool backup();

    void readNextElement(QXmlStreamReader &reader);

    void readSwitchElement(QXmlStreamReader &reader);
    void readDslamElement(QXmlStreamReader &reader);
    void readDslamBoardList(QXmlStreamReader &reader,
                             Device::Ptr deviceInfo);
    void readOltElement(QXmlStreamReader &reader);
    void readOltProfileList(QXmlStreamReader &reader,
                             Device::Ptr deviceInfo);

    void writeSwitchElement(QXmlStreamWriter &writer,
                             const Switch::Ptr &deviceInfo);
    void writeDslamElement(QXmlStreamWriter &writer,
                            const Dslam::Ptr &deviceInfo);
    void writeOltElement(QXmlStreamWriter &writer,
                          const Olt::Ptr &deviceInfo);
    void writeOltProfileList(QXmlStreamWriter &writer,
                              const OltProfileMap &profileMap, QString typeElem);

    void changeDeviceModel(int index, DeviceType::Enum deviceType,
                           DeviceModel::Enum deviceModel);
    void changeSwitchModel(int index, DeviceModel::Enum deviceModel);
    void changeDslamModel(int index, DeviceModel::Enum deviceModel);
    void changeOltModel(int index, DeviceModel::Enum deviceModel);

    void deviceListIsModified();

    bool mModified;
    QString mError;
    QString mDeviceListPath;
    QString mDeviceListBackupPath;
    QVector<Device::Ptr> mList;
};

#endif // DEVICETABLEMODEL_H
