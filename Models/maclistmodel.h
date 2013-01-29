#ifndef MACLISTMODEL_H
#define MACLISTMODEL_H

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

class MacListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MacListModel(SwitchInfo::Ptr parentDevice, QObject *parent = 0);
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
    QVector<MacInfo::Ptr> mMacList;
    SwitchInfo::Ptr mParentDevice;
};

#endif // MACLISTMODEL_H
