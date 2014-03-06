#ifndef DSLAMPORTTABLEMODEL_H
#define DSLAMPORTTABLEMODEL_H

#include <customtypes.h>
#include <improvedwidgets/improvedmessagewidget.h>
#include <snmpclient.h>
#include <devices/dslam.h>
#include <ports/xdslport.h>

class DslamPortTableModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum ColumnIndex {
        PairColumn = 0,
        PortColumn,
        StateColumn,
        DescColumn,
        ProfileColumn
    };

    explicit DslamPortTableModel(Dslam::Ptr parentDevice, ImprovedMessageWidget *messageWidget,
                                 QObject *parent = 0);

    ~DslamPortTableModel();

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

    BoardType::Enum boardType();
    void setBoardType(BoardType::Enum boardType);

    void setFirstPair(int pair);
    void setBoardIndex(int boardIndex);

    void createList();

    bool updatePortExtended(QModelIndex portIndex);
    bool updatePortBasic(QModelIndex portIndex);
    bool updatePortBasic(const XdslPort::Ptr &port);
    void update();

    bool changePortState(int portIndex, int portState);
    bool changePortProfile(QModelIndex portIndex, QString profileName);

    QString error() const;

signals:
    void updateFinished(bool withError);

private:
    int currentPort(QModelIndex index);
    QVariant topLevelData(QModelIndex index) const;
    QVariant secondLevelData(QModelIndex index) const;
    QVariant secondLevelDataAdsl(QModelIndex index) const;
    QVariant secondLevelDataShdsl(QModelIndex index) const;

    void finishAsyncUpdate();

    inline void appendUpdateError(QString error) {
        mMutexUpdateErrors.lock();
        mUpdateErrors += error + "\n";
        mMutexUpdateErrors.unlock();
    }

    QString mError;
    int mFirstPair;
    int mBoardIndex;
    BoardType::Enum mBoardType;
    Dslam::Ptr mParentDevice;

    QFutureWatcher<void> *mFutureWatcher;
    QString mUpdateErrors;
    QMutex mMutexUpdateErrors;

    QVector<XdslPort::Ptr> mList;
    ImprovedMessageWidget *mMessageWidget;
};

//Wrapper for QtConcurrent::map
struct DslamUpdateWrapperObject
{
    DslamUpdateWrapperObject(DslamPortTableModel *instance)
    : mInstance(instance) { }

    typedef void result_type;

    void operator()(const XdslPort::Ptr &port)
    {
         mInstance->updatePortBasic(port);
    }

    DslamPortTableModel *mInstance;
};

#endif // DSLAMPORTTABLEMODEL_H
