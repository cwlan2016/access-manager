#ifndef MACTABLEMODEL_H
#define MACTABLEMODEL_H

#include <snmpclient.h>
#include <devices/switch.h>
#include <other-data/mac.h>

class MacTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum ColumnIndex {
        PortColumn = 0,
        VlanColumn,
        MacAddressColumn
    };

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
    QString vlanValue(const int macIndex) const;
    QVector<Mac::Ptr> *asyncUpdateMacTable();

    void finishAsyncUpdate();

    QString decMacAddressToHex(oid *macAddressOid, int length);

    QFutureWatcher<QVector<Mac::Ptr> *> *mFutureWatcher;
    QString mUpdateErrors;
    QString mError;
    QVector<Mac::Ptr> *mList;
    Switch::Ptr mParentDevice;
};

#endif // MACTABLEMODEL_H
