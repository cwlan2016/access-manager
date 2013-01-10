#ifndef DSLAMPORTLISTMODEL_H
#define DSLAMPORTLISTMODEL_H

#include <QtCore/QAbstractItemModel>
#ifdef _MSC_VER
#include "../customtypes.h"
#include "../snmpclient.h"
#include "../Info/xdslportinfo.h"
#else
#include "customtypes.h"
#include "snmpclient.h"
#include "Info/xdslportinfo.h"
#endif

class DslamPortListModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit DslamPortListModel(DeviceModel::Enum deviceModel, QString ip, QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    QString error() const;
    BoardType::Enum boardType();
    void setBoardType(BoardType::Enum boardType);
    void setFirstPair(int pair);
    void setBoardNumber(int boardNum);
    bool load();
    void createList();
    bool updatePortInfo(QModelIndex index);
    bool changePortState(int portIndex, QString portState);
    bool changePortProfile(QModelIndex portIndex, QString profileName);
private:
    QString mError;
    BoardType::Enum mBoardType;
    int mBoardNumber;
    int mFirstPair;
    QString mIp;
    DeviceModel::Enum mDeviceModel;
    std::vector<XdslPortInfo::Ptr> mPortList;

    QVariant topLevelData(QModelIndex index) const;
    QVariant secondLevelData(QModelIndex index) const;
    void updatePortMA(QModelIndex portIndex, QScopedPointer<SnmpClient> &snmp);
    void updatePortMXA(QModelIndex portIndex, QScopedPointer<SnmpClient> &snmp);
    int currentPort(QModelIndex index);
};

#endif // DSLAMPORTLISTMODEL_H
