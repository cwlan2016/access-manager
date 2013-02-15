#ifndef BOARDTABLEMODEL_H
#define BOARDTABLEMODEL_H

#ifdef _MSC_VER
#include "../basicdialogs.h"
#include "../Info/boardinfo.h"
#include "../Info/dslaminfo.h"
#else
#include "basicdialogs.h"
#include "Info/boardinfo.h"
#include "Info/dslaminfo.h"
#endif

class BoardTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit BoardTableModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool removeRow(int row, const QModelIndex &parent);

    QHash<int, BoardInfo> boardList() const;
    void setBoardList(QHash<int, BoardInfo> &boardList);

    DslamInfo::Ptr parentDevice();
    void setParentDevice(const DslamInfo::Ptr &parent);

    bool getBoardListFromDevice();
    void renumeringPairList();

    QString error() const;

private:
    QString rangePairs(int firstPair, BoardType::Enum typeBoard) const;

    QString mError;
    DslamInfo::Ptr mParentDevice;
    QHash<int, BoardInfo> mList;
};

#endif // BOARDTABLEMODEL_H
