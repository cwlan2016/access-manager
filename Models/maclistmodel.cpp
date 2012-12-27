#include "maclistmodel.h"

#ifdef _MSC_VER
#include "../constant.h"
#include "../converters.h"
#else
#include "constant.h"
#include "converters.h"
#endif

// Columns
// DisplayRole
// 0 - number port
// 1 - vlan name
// 2 - mac

MacListModel::MacListModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int MacListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return mMacList.size();
}

int MacListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 3;
}

QVariant MacListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole) {
        if (index.column() == 0)
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        else
            return int(Qt::AlignCenter | Qt::AlignVCenter);
    } else if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (index.column() == 0)
            return mMacList[index.row()]->numberPort();
        else if (index.column() == 1)
            return mMacList[index.row()]->vlanName();
        else if (index.column() == 2)
            return mMacList[index.row()]->mac();
        else
            return QVariant();
    } else
        return QVariant();
}

QVariant MacListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical)
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (section == 0)
            return MacListModelColumn::Port;
        else if (section == 1)
            return MacListModelColumn::Vlan;
        else if (section == 2)
            return MacListModelColumn::MacAddress;
        else
            return QVariant();
    } else if (role == Qt::TextAlignmentRole) {
        return int(Qt::AlignCenter | Qt::AlignVCenter);
    } else if (role == Qt::FontRole) {
        QFont font(qApp->font());
        font.setBold(true);
        return font;
    }

    return QVariant();
}

Qt::ItemFlags MacListModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool MacListModel::updateMacTable()
{
    mMacList.clear();

    SnmpClient *snmp = new SnmpClient();

    snmp->setIP(mIp);

    if (!snmp->setupSession(SessionType::ReadSession)) {
        mError = SnmpErrors::SetupSession;
        delete snmp;
        return false;
    }

    if (!snmp->openSession()) {
        mError = SnmpErrors::OpenSession;
        delete snmp;
        return false;
    }

    updateMacTableVlan(snmp, mInetVlanTag, "Inet");
    updateMacTableVlan(snmp, mIptvVlanTag, "IPTV Unicast");

    delete snmp;

    reset();

    return true;
}

void MacListModel::setIP(QString ip)
{
    mIp = ip;
}

void MacListModel::setInetVlanTag(int vlanTag)
{
    mInetVlanTag = vlanTag;
}

void MacListModel::setIptvVlanTag(int vlanTag)
{
    mIptvVlanTag = vlanTag;
}

QString MacListModel::error() const
{
    return mError;
}

void MacListModel::updateMacTableVlan(SnmpClient *snmp, quint32 vlanTag, QString vlanName)
{
    QString oidString = Mib::dot1qTpFdbPort % QString::number(vlanTag);

    oid vlanMacOid[14];
    size_t lenVlanNameOid = 14;
    snmp_parse_oid(oidString.toLatin1().data(), vlanMacOid, &lenVlanNameOid);

    oid *nextOid = new oid[14];
    size_t nextOid_len = 14;
    snmp_parse_oid(oidString.toLatin1().data(), nextOid, &nextOid_len);

    while (true) {
        snmp->createPdu(SNMP_MSG_GETNEXT);
        snmp->addOid(nextOid, nextOid_len);

        if (!snmp->sendRequest())
            break;

        netsnmp_variable_list *vars = snmp->varList();

        if (snmp_oid_ncompare(vlanMacOid, lenVlanNameOid, vars->name, vars->name_length, 14) != 0)
            break;

        MacInfo::Ptr macInfo = std::make_shared<MacInfo>();
        mMacList.push_back(std::move(macInfo));

        mMacList[mMacList.size() - 1]->setNumberPort(*(vars->val.integer));
        mMacList[mMacList.size() - 1]->setVlanName(vlanName);
        mMacList[mMacList.size() - 1]->setMac(DecMacAddressToHex(vars->name, vars->name_length));

        delete[] nextOid;
        nextOid = new oid[vars->name_length];

        memmove(nextOid, vars->name, vars->name_length * sizeof(oid));
        nextOid_len = vars->name_length;

        snmp->clearResponsePdu();
    }

    delete[] nextOid;
}
