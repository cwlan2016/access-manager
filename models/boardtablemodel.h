#ifndef BOARDTABLEMODEL_H
#define BOARDTABLEMODEL_H

#include <basicdialogs.h>
#include <other-data/dslamboard.h>
#include <devices/dslam.h>

class BoardTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum ColumnIndex {
            NumberColumn = 0,
            TypeBoardColumn,
            PairsColumn
    };

    explicit BoardTableModel(Dslam *parentDevice, QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool removeRow(int row, const QModelIndex &parent);

    QHash<int, DslamBoard::Ptr> boardList() const;
    void addBoard(int index, BoardType::Enum type, int firstPair);
    DslamBoard::Ptr newBoard(int index, BoardType::Enum type, int firstPair);

    Dslam *parentDevice();

    bool getBoardListFromDevice();
    void renumeringPairList();

    QString error() const;
signals:
    void modified();
private:
    QString rangePairs(int firstPair, BoardType::Enum typeBoard) const;
    void boardIsModified();

    QString mError;
    Dslam *mParentDevice;
    QHash<int, DslamBoard::Ptr> mList;
};

#endif // BOARDTABLEMODEL_H
