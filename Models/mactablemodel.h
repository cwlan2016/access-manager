#ifndef MACTABLEMODEL_H
#define MACTABLEMODEL_H

#include <QtCore/QAbstractTableModel>
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
    explicit MacTableModel(SwitchInfo::Ptr parentDevice, QObject *parent = 0);
    ~MacTableModel();
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent) const;
    QString error() const;
    bool update();
private:
    void updateMacInVlan(QScopedPointer<SnmpClient> &snmpClient, long vlanTag, QString vlanName);
    QString mError;
    QVector<MacInfo::Ptr> *mList;
    SwitchInfo::Ptr mParentDevice;
};

#endif // MACTABLEMODEL_H
