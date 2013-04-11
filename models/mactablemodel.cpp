#include "mactablemodel.h"

#ifdef _MSC_VER
#include "../constant.h"
#include "../converters.h"
#include "../customsnmpfunctions.h"
#include "../configs/switchconfig.h"
#else
#include "constant.h"
#include "converters.h"
#include "customsnmpfunctions.h"
#include "configs/switchconfig.h"
#endif

// Columns
// DisplayRole
// 0 - number port
// 1 - vlan name
// 2 - mac

MacTableModel::MacTableModel(Switch::Ptr parentDevice, QObject *parent) :
    QAbstractTableModel(parent),
    mList(0),
    mParentDevice(parentDevice)
{
}

MacTableModel::~MacTableModel()
{
    if (mList)
        delete mList;
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

bool MacTableModel::update()
{
    QScopedPointer<SnmpClient> snmp(new SnmpClient());

    snmp->setIp(mParentDevice->ip());

    if (!snmp->setupSession(SessionType::ReadSession)) {
        mError = SnmpErrorStrings::SetupSession;
        return false;
    }

    if (!snmp->openSession()) {
        mError = SnmpErrorStrings::OpenSession;
        return false;
    }

    beginResetModel();

    if (mList)
        delete mList;

    mList = new QVector<Mac::Ptr>();
    mList->reserve(26);

    //TODO: Make handling errors
    updateMacsInVlan(snmp, mParentDevice->inetVlanTag(),
                     SwitchConfig::inetVlanName());
    updateMacsInVlan(snmp, mParentDevice->iptvVlanTag(),
                     SwitchConfig::iptvVlanName());

    endResetModel();

    return true;
}

QString MacTableModel::error() const
{
    return mError;
}

void MacTableModel::updateMacsInVlan(QScopedPointer<SnmpClient> &snmpClient,
                                     long vlanTag, QString vlanName)
{
    oid *vlanMacOid = createOid(Mib::dot1qTpFdbPort, 13, vlanTag);
    size_t lenVlanNameOid = 14;

    snmpClient->createPdu(SNMP_MSG_GETNEXT);
    snmpClient->addOid(vlanMacOid, lenVlanNameOid);

    while (snmpClient->sendRequest()) {
        netsnmp_variable_list *vars = snmpClient->varList();

        if (snmp_oid_ncompare(vlanMacOid, lenVlanNameOid, vars->name,
                              vars->name_length, lenVlanNameOid) != 0)
            break;

        Mac::Ptr macInfo = new Mac(this);
        macInfo->setPort(*(vars->val.integer));
        macInfo->setVlanName(vlanName);
        macInfo->setMac(decMacAddressToHex(vars->name, vars->name_length));

        mList->push_back(macInfo);

        snmpClient->createPduFromResponse(SNMP_MSG_GETNEXT);
    }
}
