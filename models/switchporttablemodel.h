#ifndef SWITCHPORTTABLEMODEL_H
#define SWITCHPORTTABLEMODEL_H

#include <customtypes.h>
#include <snmpclient.h>
#include <devices/switch.h>
#include <improvedwidgets/improvedmessagewidget.h>
#include <ports/switchport.h>

class SwitchPortTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum ColumnIndex {
        NumberColumn = 0,
        StateColumn,
        SpeedDuplexColumn,
        DescColumn
    };

    explicit SwitchPortTableModel(Switch::Ptr parentDevice, ImprovedMessageWidget *messageWidget,
                                  QObject *parent = 0);

    ~SwitchPortTableModel();

    int rowCount(const QModelIndex &parentDevice) const;
    int columnCount(const QModelIndex &parentDevice) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool memberMulticastVlan(int port);
    bool setMemberMulticastVlan(int port, bool value);

    VlanState::Enum memberInetVlan(int port);
    VlanState::Enum memberIptvVlan(int port);

    bool setMemberInternetService(int port);
    bool setMemberInternetWithIptvStbService(int port);

    bool updatePort(int index);
    bool updatePort(const SwitchPort::Ptr &port);
    void update();
    bool updateIsRunning();

    bool getVlanSettings();

    QString error() const;

signals:
    void updateFinished(bool withError);

private:
    void createList();

    bool getUnicastVlanSettings(const OidPair &oidVlan,
                                QBitArray &vlanPortArray, QString vlanName);
    bool getMulticastVlanSettings();

    bool sendVlanSetting(QVector<OidPair> &oidStringList,
                         QList<QBitArray> &arrayList, bool ismv);

    void finishAsyncUpdate();

    inline void appendUpdateError(QString error) {
        mMutexUpdateErrors.lock();
        mUpdateErrors += error + "\n";
        mMutexUpdateErrors.unlock();
    }

    QBitArray ucharToQBitArray(DeviceModel::Enum deviceModel, uchar *str);
    QString qbitArrayToHexString(DeviceModel::Enum deviceModel,
                                 const QBitArray &array, bool ismv);

    QString mError;
    int mIptvMultVlanTag;
    Switch::Ptr mParentDevice;
    QBitArray mInetVlanAllMember;
    QBitArray mInetVlanUntagMember;
    QBitArray mMulticastVlanMember;
    QBitArray mIptvUnicastVlanAllMember;
    QBitArray mIptvUnicastVlanUntagMember;

    QFutureWatcher<void> *mFutureWatcher;
    QString mUpdateErrors;
    QMutex mMutexUpdateErrors;

    QVector<SwitchPort::Ptr> mList;
    ImprovedMessageWidget *mMessageWidget;
};

//Wrapper for QtConcurrent::map
struct SwitchUpdateWrapperObject
{
    SwitchUpdateWrapperObject(SwitchPortTableModel *instance)
    : mInstance(instance) {
    }

    typedef void result_type;

    void operator()(const SwitchPort::Ptr &port) {
         mInstance->updatePort(port);
    }

    SwitchPortTableModel *mInstance;
};

#endif // SWITCHPORTABLEMODEL_H
