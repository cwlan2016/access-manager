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

    void update();
    bool updateIsRunning();

    QString error() const;

signals:
    void updateFinished(bool withError);

private:
    bool updateMacsInVlan(QScopedPointer<SnmpClient> &snmpClient,
                          QVector<Mac::Ptr> *list, long vlanTag, QString vlanName);
    QVector<Mac::Ptr> *asyncUpdateMacTable(bool fullMacTable);

    void finishAsyncUpdate();

    QString decMacAddressToHex(oid *macAddressOid, int length);

    QFuture<QVector<Mac::Ptr> *> *mFuture;
    QFutureWatcher<QVector<Mac::Ptr> *> *mFutureWatcher;
    QString mError;
    QVector<Mac::Ptr> *mList;
    Switch::Ptr mParentDevice;
};

#endif // MACTABLEMODEL_H
