#ifndef BOARDLISTMODEL_H
#define BOARDLISTMODEL_H

#include <QtCore/QAbstractTableModel>

#ifdef _MSC_VER
#include "../basicdialogs.h"
#include "../Info/boardinfo.h"
#include "../Info/deviceinfo.h"
#else
#include "basicdialogs.h"
#include "Info/boardinfo.h"
#include "Info/deviceinfo.h"
#endif


class BoardListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit BoardListModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool removeRow(int row, const QModelIndex &parent);
    //настройка модели
    QHash<int, BoardInfo> boardList() const;
    short autoFill() const;
    short autoNumeringBoard() const;
    DeviceInfo::Ptr parentDevice();
    void setBoardList(QHash<int, BoardInfo> &boardList);
    void setAutoFill(short autoFill);
    void setAutoNumeringBoard(short autoNumeringBoard);
    void setParentDevice(const DeviceInfo::Ptr &parent);
    bool getBoardListFromDevice();
    void renumeringPairList();
    QString error() const;
private:
    QHash<int, BoardInfo> mBoardList;
    QString mError;
    //TODO: Remove this 2 fields. its consist mParentDevice
    short mAutoFill;
    short mAutoNumeringBoard;
    DeviceInfo::Ptr mParentDevice;
    QString rangePairs(int firstPair, BoardType::Enum typeBoard) const;
};

#endif // BOARDLISTMODEL_H
