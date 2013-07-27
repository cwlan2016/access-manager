#include "mactablemodel.h"

#include <constant.h>
#include <converters.h>
#include <customsnmpfunctions.h>
#include <configs/switchconfig.h>

// Columns
// DisplayRole
// 0 - number port
// 1 - vlan name
// 2 - mac

MacTableModel::MacTableModel(Switch::Ptr parentDevice, QObject *parent) :
    QAbstractTableModel(parent),
    mFuture(new QFuture<QVector<Mac::Ptr> *>()),
    mFutureWatcher(new QFutureWatcher<QVector<Mac::Ptr> *>()),
    mList(new QVector<Mac::Ptr>()),
    mParentDevice(parentDevice)
{
    connect(mFutureWatcher, &QFutureWatcher<QVector<Mac::Ptr> *>::finished,
            this, &MacTableModel::finishAsyncUpdate);
}

MacTableModel::~MacTableModel()
{
    if (mList)
        delete mList;

    if (mFutureWatcher) {
        if (mFutureWatcher->isRunning())
            mFutureWatcher->waitForFinished();
        delete mFutureWatcher;
    }

    if (mFuture)
        delete mFuture;
}

int MacTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return mList->size();
}

int MacTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 3;
}

QVariant MacTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole) {
        if (index.column() == 0) {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        } else {
            return int(Qt::AlignCenter | Qt::AlignVCenter);
        }
    } else if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (index.column() == 0) {
            return mList->at(index.row())->port();
        } else if (index.column() == 1) {
            return mList->at(index.row())->vlanName();
        } else if (index.column() == 2) {
            return mList->at(index.row())->mac();
        }
    }

    return QVariant();
}

QVariant MacTableModel::headerData(int section, Qt::Orientation orientation,
                                   int role) const
{
    if (orientation == Qt::Vertical)
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (section == 0) {
            return MacTableModelStrings::Port;
        } else if (section == 1) {
            return MacTableModelStrings::Vlan;
        } else if (section == 2) {
            return MacTableModelStrings::MacAddress;
        }
    } else if (role == Qt::TextAlignmentRole) {
        return int(Qt::AlignCenter | Qt::AlignVCenter);
    } else if (role == Qt::FontRole) {
        QFont font(qApp->font());
        font.setBold(true);
        return font;
    }

    return QVariant();
}

Qt::ItemFlags MacTableModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void MacTableModel::update()
{
    if (mFutureWatcher->isRunning()) {
        mError = QString::fromUtf8("Обновление таблицы MAC-адресов уже выполняется.");
        emit updateFinished(true);
        return;
    }

    *mFuture = QtConcurrent::run(this, &MacTableModel::asyncUpdateMacTable, false);
    mFutureWatcher->setFuture(*mFuture);
}

QString MacTableModel::error() const
{
    return mError;
}

bool MacTableModel::updateMacsInVlan(QScopedPointer<SnmpClient> &snmpClient,
                                     QVector<Mac::Ptr> *list,
                                     long vlanTag, QString vlanName)
{
    OidPair oidPair = createOidPair(Mib::dot1qTpFdbPort, 13, vlanTag);

    snmpClient->createPdu(SNMP_MSG_GETNEXT);
    snmpClient->addOid(oidPair);

    while (true) {
        if (!snmpClient->sendRequest())
            return false;

        netsnmp_variable_list *vars = snmpClient->varList();

        if (!isValidSnmpValue(vars))
            return false;

        if (snmp_oid_ncompare(oidPair.first, oidPair.second, vars->name,
                              vars->name_length, oidPair.second) != 0)
            break;

        Mac::Ptr macInfo = new Mac(0);          //create without parent
        macInfo->moveToThread(this->thread());  //move to main gui thread
        macInfo->setParent(this);               //set model as parent
        macInfo->setPort(*(vars->val.integer));
        macInfo->setVlanName(vlanName);
        macInfo->setMac(decMacAddressToHex(vars->name, vars->name_length));

        list->push_back(macInfo);

        snmpClient->createPduFromResponse(SNMP_MSG_GETNEXT);
    }

    return true;
}

QVector<Mac::Ptr> *MacTableModel::asyncUpdateMacTable(bool fullMacTable)
{
    Q_UNUSED(fullMacTable)

    QScopedPointer<SnmpClient> snmp(new SnmpClient());

    snmp->setIp(mParentDevice->ip());

    if (!snmp->setupSession(SessionType::ReadSession)) {
        mError = SnmpErrorStrings::SetupSession;
        return 0;
    }

    if (!snmp->openSession()) {
        mError = SnmpErrorStrings::OpenSession;
        return 0;
    }

    QVector<Mac::Ptr> *newList = new QVector<Mac::Ptr>();
    newList->reserve(26);

    bool result = updateMacsInVlan(snmp, newList, mParentDevice->inetVlanTag(),
                                   SwitchConfig::inetVlanName());
    if (!result) {
        mError = snmp->error();
        qDeleteAll(newList->begin(), newList->end());
        delete newList;
        return 0;
    }

    result = updateMacsInVlan(snmp, newList, mParentDevice->iptvVlanTag(),
                              SwitchConfig::iptvVlanName());
    if (!result) {
        mError = snmp->error();
        qDeleteAll(newList->begin(), newList->end());
        delete newList;
        return 0;
    }

    return newList;
}

bool MacTableModel::updateIsRunning()
{
    return mFutureWatcher->isRunning();
}

void MacTableModel::finishAsyncUpdate()
{
    auto list = mFutureWatcher->result();
    if (list) {
        beginResetModel();

        if (mList) {
            qDeleteAll(mList->begin(), mList->end());
            delete mList;
        }

        mList = list;

        endResetModel();
        emit updateFinished(false);
    } else {
        emit updateFinished(true);
    }
}

QString MacTableModel::decMacAddressToHex(oid *macAddressOid, int length)
{
    QString tempStr = "";

    for (int i = 14; i < length; ++i) {
        if (i != 14) {
            tempStr = QString("%1-%2").arg(tempStr).arg(macAddressOid[i], 2, 16, QLatin1Char('0'));
        } else {
            tempStr = QString("%1").arg(macAddressOid[i], 2, 16, QLatin1Char('0'));
        }
    }

    return tempStr.toUpper();
}
