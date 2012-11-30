#ifndef DSLAMPORTLISTMODEL_H
#define DSLAMPORTLISTMODEL_H

#include "stdafx.h"

#include "constant.h"
#include "snmpclient.h"
#include "customsnmpfunctions.h"

#include "Info/deviceinfo.h"
#include "Info/adslportinfo.h"
#include "Info/shdslportinfo.h"

#include "Models/boardlistmodel.h"

class DslamPortListModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit DslamPortListModel(DeviceModel deviceModel, QString ip, QObject* parent = 0);
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    QModelIndex index(int row, int column, const QModelIndex& parent) const;
    QModelIndex parent(const QModelIndex& child) const;
    QString error() const;
    BoardType boardType();
    void setBoardType(BoardType boardType);
    void setFirstPair(int pair);
    void setBoardNumber(int boardNum);
    bool load();
    void createList();
    bool updatePortInfo(QModelIndex index);
    bool changePortState(int portIndex, QString portState);
    bool changePortProfile(QModelIndex portIndex, QString profileName);
private:
    QString mError;
    BoardType mBoardType;
    int mBoardNumber;
    int mFirstPair;
    QString mIp;
    DeviceModel mDeviceModel;
    std::vector<XdslPortInfo::Ptr> mPortList;

    QVariant topLevelData(QModelIndex index) const;
    QVariant secondLevelData(QModelIndex index) const;
    void updatePortMA(QModelIndex portIndex, std::unique_ptr<SnmpClient>& snmp);
    void updatePortMXA(QModelIndex portIndex, std::unique_ptr<SnmpClient>& snmp);
    int currentPort(QModelIndex index);
};

#endif // DSLAMPORTLISTMODEL_H
