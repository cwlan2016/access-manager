#ifndef MACTABLEMODEL_H
#define MACTABLEMODEL_H

#ifdef _MSC_VER
#include "../Info/macinfo.h"
#include "../Info/switchinfo.h"
#include "../snmpclient.h"
#else
#include "Info/macinfo.h"
#include "Info/switchinfo.h"
#include "snmpclient.h"
#endif

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

    QString mError;
    QVector<Mac::Ptr> *mList;
    Switch::Ptr mParentDevice;
};

#endif // MACTABLEMODEL_H
