#ifndef MACTABLEMODEL_H
#define MACTABLEMODEL_H

#include <snmpclient.h>
#include <devices/switch.h>
#include <other-data/mac.h>

class MacTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MacTableModel(Switch::Ptr parentDevice, QObject *parent = 0);
    ~MacTableModel();

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool update();

    QString error() const;

private:
    void updateMacsInVlan(QScopedPointer<SnmpClient> &snmpClient, long vlanTag,
                          QString vlanName);
    QString decMacAddressToHex(oid *macAddressOid, int length);

    QString mError;
    QVector<Mac::Ptr> *mList;
    Switch::Ptr mParentDevice;
};

#endif // MACTABLEMODEL_H
