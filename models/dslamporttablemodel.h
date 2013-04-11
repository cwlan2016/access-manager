#ifndef DSLAMPORTTABLEMODEL_H
#define DSLAMPORTTABLEMODEL_H

#ifdef _MSC_VER
#include "../customtypes.h"
#include "../snmpclient.h"
#include "../Info/dslaminfo.h"
#include "../Info/xdslportinfo.h"
#else
#include "customtypes.h"
#include "snmpclient.h"
#include "Info/dslaminfo.h"
#include "Info/xdslportinfo.h"
#endif

class DslamPortTableModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit DslamPortTableModel(Dslam::Ptr parentDevice,
                                 QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

    BoardType::Enum boardType();
    void setBoardType(BoardType::Enum boardType);

    void setFirstPair(int pair);
    void setBoardIndex(int boardIndex);

    bool load();
    void createList();

    bool updatePortInfo(QModelIndex index);
    bool changePortState(int portIndex, int portState);
    bool changePortProfile(QModelIndex portIndex, QString profileName);

    QString error() const;

private:
    int currentPort(QModelIndex index);
    QVariant topLevelData(QModelIndex index) const;
    QVariant secondLevelData(QModelIndex index) const;
    bool updatePortMA(QModelIndex portIndex, QScopedPointer<SnmpClient> &snmp);
    bool updatePortMXA(QModelIndex portIndex, QScopedPointer<SnmpClient> &snmp);

    QString mError;
    int mFirstPair;
    int mBoardIndex;
    BoardType::Enum mBoardType;
    Dslam::Ptr mParentDevice;
    QVector<XdslPort::Ptr> mList;
};

#endif // DSLAMPORTTABLEMODEL_H
