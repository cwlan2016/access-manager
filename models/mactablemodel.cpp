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
        if (index.column() == PortColumn) {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        } else {
            return int(Qt::AlignCenter | Qt::AlignVCenter);
        }
    } else if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case PortColumn:
            return mList->at(index.row())->port();
        case VlanColumn:
            return vlanValue(index.row());
        case MacAddressColumn:
            return mList->at(index.row())->mac();
        default:
            return QVariant();
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
        switch (section) {
        case PortColumn:
            return MacTableModelStrings::Port;
        case VlanColumn:
            return MacTableModelStrings::Vlan;
        case MacAddressColumn:
            return MacTableModelStrings::MacAddress;
        default:
            return QVariant();
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

    mUpdateErrors = "";
    QFuture<QVector<Mac::Ptr> *> future = QtConcurrent::run(this, &MacTableModel::asyncUpdateMacTable);
    mFutureWatcher->setFuture(future);
}

QString MacTableModel::error() const
{
    return mError;
}

QString MacTableModel::vlanValue(const int macIndex) const
{
    int vlan = mList->at(macIndex)->vlan();

    if (vlan == mParentDevice->inetVlanTag()) {
        return QString("Inet (%1)").arg(vlan);
    } else if (vlan == mParentDevice->iptvVlanTag()) {
        return QString("IPTV Unicast (%1)").arg(vlan);
    }

    return QString::number(vlan);
}

QVector<Mac::Ptr> *MacTableModel::asyncUpdateMacTable()
{
    QScopedPointer<SnmpClient> snmp(new SnmpClient());

    snmp->setIp(mParentDevice->ip());

    if (!snmp->setupSession(SessionType::ReadSession)) {
        mUpdateErrors = SnmpErrorStrings::SetupSession;
        return 0;
    }

    if (!snmp->openSession()) {
        mUpdateErrors = SnmpErrorStrings::OpenSession;
        return 0;
    }

    QVector<Mac::Ptr> *newList = new QVector<Mac::Ptr>();
    newList->reserve(26);

    OidPair oidPair = createOidPair(Mib::dot1qTpFdbPort, 13);

    while (true) {
        snmp->createPdu(SNMP_MSG_GETBULK, 10);
        snmp->addOid(oidPair);

        if (!snmp->sendRequest()) {
            mUpdateErrors = snmp->error();
            break;
        }

        netsnmp_variable_list *vars = snmp->varList();

        while (true) {
            if (!isValidSnmpValue(vars)) {
                mUpdateErrors = snmp->error();
                return newList;
            }

            if (snmp_oid_ncompare(oidPair.first, oidPair.second, vars->name,
                                  vars->name_length, 13) != 0) {
                return newList;
            }

            Mac::Ptr macInfo = new Mac(0);          //create without parent
            macInfo->setPort(*(vars->val.integer));
            macInfo->setVlan(vars->name[vars->name_length - 7]); //index = vlan.mac
            macInfo->setMac(decMacAddressToHex(vars->name, vars->name_length));
            newList->push_back(macInfo);

            if (!vars->next_variable) {
                oidPair = createOidPair(vars->name, vars->name_length);
                break;
            }

            vars = vars->next_variable;
        }
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

        auto end = mList->end();

        for (auto it = mList->begin(); it != end; ++it) {
            (*it)->moveToThread(this->thread());
            (*it)->setParent(this);
        }

        endResetModel();
    }

    if (!mUpdateErrors.isEmpty()) {
        mError = mUpdateErrors;
        emit updateFinished(true);
    } else {
        emit updateFinished(false);
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
