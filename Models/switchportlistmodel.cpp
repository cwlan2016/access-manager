#include "switchportlistmodel.h"

#include <QtCore/QStringBuilder>
#include <QtGui/QFont>
#include <QtWidgets/QApplication>
#ifdef _MSC_VER
#include "../basicdialogs.h"
#include "../constant.h"
#include "../converters.h"
#include "../customsnmpfunctions.h"
#else
#include "basicdialogs.h"
#include "constant.h"
#include "converters.h"
#include "customsnmpfunctions.h"
#endif

// Columns
// DisplayRole
// 0 - number
// 1 - operation state
// 2 - speed/duplex
// 3 - description

SwitchPortListModel::SwitchPortListModel(SwitchInfo::Ptr parentDevice, QObject *parent) :
    QAbstractTableModel(parent),
    mParentDevice(parentDevice)
{
    createList();

    mIptvMultVlanTag = 0;

    int countBit;

    if ((mParentDevice->deviceModel() == DeviceModel::DES3526)
            || (mParentDevice->deviceModel() == DeviceModel::DES3528)) {
        countBit = 32;
    } else if (mParentDevice->deviceModel() == DeviceModel::DES3550) {
        countBit = 56;
    } else {
        countBit = 32;
    }

    mInetVlanAllMember.fill(false, countBit);
    mInetVlanUntagMember.fill(false, countBit);
    mIptvUnicastVlanAllMember.fill(false, countBit);
    mIptvUnicastVlanUntagMember.fill(false, countBit);
    mMulticastVlanMember.fill(false, countBit);
}

int SwitchPortListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return mList.size();
}

int SwitchPortListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 4;
}

QVariant SwitchPortListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::TextAlignmentRole) {
        if (index.column() == 0) {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        } else {
            return int(Qt::AlignCenter | Qt::AlignVCenter);
        }
    } else if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (index.column() == 0) {
            return mList.at(index.row())->number();
        } else if (index.column() == 1) {
            return mList.at(index.row())->state();
        } else if (index.column() == 2) {
            return mList.at(index.row())->speedDuplex();
        } else if (index.column() == 3) {
            return mList.at(index.row())->desc();
        } else {
            return QVariant();
        }
    } else if ((role == Qt::BackgroundColorRole) && (index.internalId() == 0)) {
        if (mList.at(index.row())->state() == "Down") {
            return QBrush(QColor(223, 255, 252));
        } else if (mList.at(index.row())->state() == "Up") {
            return QBrush(QColor(200, 255, 200));
        } else {
            return QVariant();
        }
    } else {
        return QVariant();
    }
}

QVariant SwitchPortListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical)
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (section == 0)
            return SwitchPortListModelColumn::Number;
        else if (section == 1)
            return SwitchPortListModelColumn::State;
        else if (section == 2)
            return SwitchPortListModelColumn::SpeedDuplex;
        else if (section == 3)
            return SwitchPortListModelColumn::Desc;
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

void SwitchPortListModel::sort(int column, Qt::SortOrder order)
{
    beginResetModel();

    std::sort(mList.begin(), mList.end(),
              [&](const SwitchPortInfo::Ptr &first,
    const SwitchPortInfo::Ptr &second) -> bool {
        if (column == 0)
            return order == Qt::AscendingOrder ? (first->number() < second->number()) : (first->number() > second->number());
        else if (column == 1)
            return order == Qt::AscendingOrder ? (first->state() < second->state()) : (first->state() > second->state());
        else if (column == 2)
            return order == Qt::AscendingOrder ? (first->speedDuplex() < second->speedDuplex()) : (first->speedDuplex() > second->speedDuplex());
        else if (column == 3)
            return order == Qt::AscendingOrder ? (first->desc() < second->desc()) : (first->desc() > second->desc());
        return false;
    });

    endResetModel();
}

Qt::ItemFlags SwitchPortListModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QString SwitchPortListModel::error() const
{
    return mError;
}

bool SwitchPortListModel::memberMulticastVlan(int port)
{
    return mMulticastVlanMember.at(port - 1);
}

VlanState::Enum SwitchPortListModel::memberInetVlan(int port)
{
    if (mInetVlanUntagMember.at(port - 1)) {
        return VlanState::Untag;
    } else if (mInetVlanAllMember.at(port - 1)) {
        return VlanState::Tag;
    } else {
        return VlanState::None;
    }
}

VlanState::Enum SwitchPortListModel::memberIptvVlan(int port)
{
    if (mIptvUnicastVlanUntagMember.at(port - 1)) {
        return VlanState::Untag;
    } else if (mIptvUnicastVlanAllMember.at(port - 1)) {
        return VlanState::Tag;
    } else {
        return VlanState::None;
    }
}

bool SwitchPortListModel::setMemberMulticastVlan(int port, bool value)
{
    if (!getMulticastVlanSettings()) {
        return false;
    }

    if (mMulticastVlanMember.at(port - 1) == value) {
        if (value) {
            mError = QString::fromUtf8("Ошибка: Порт %1 уже является членом Multicast влана.")
                     .arg(QString::number(port));
        } else {
            mError = QString::fromUtf8("Ошибка: Порт %1 не является членом Multicast влана.")
                     .arg(QString::number(port));
        }

        return false;
    } else {
        mMulticastVlanMember.setBit(port - 1, value);
    }

    QVector<OidPair> oidList;

    if (mParentDevice->deviceModel() == DeviceModel::DES3526)
        oidList.push_back(OidPair(createOid(Mib::swL2IGMPMulticastVlanMemberPortDES3526, 15, mIptvMultVlanTag), 16));
    else if (mParentDevice->deviceModel() == DeviceModel::DES3550)
        oidList.push_back(OidPair(createOid(Mib::swL2IGMPMulticastVlanMemberPortDES3550, 15, mIptvMultVlanTag), 16));
    else
        oidList.push_back(OidPair(createOid(Mib::swL2IGMPMulticastVlanMemberPortDES3528, 13, mIptvMultVlanTag), 14));

    QList<QBitArray> arrayList;
    arrayList.push_back(mMulticastVlanMember);

    return sendVlanSetting(oidList, arrayList, true);
}

bool SwitchPortListModel::setMemberInternetService(int port)
{
    bool result = getUnicastVlanSettings(createOid(Mib::dot1qVlanStaticEgressPorts, 13, mParentDevice->inetVlanTag()),
                                         14, mInetVlanAllMember, "Inet");

    if (!result)
        return false;

    result = getUnicastVlanSettings(createOid(Mib::dot1qVlanStaticUntaggedPorts, 13, mParentDevice->inetVlanTag()),
                                    14, mInetVlanUntagMember, "Inet");

    if (!result)
        return false;

    result = getUnicastVlanSettings(createOid(Mib::dot1qVlanStaticEgressPorts, 13, mParentDevice->iptvVlanTag()),
                                    14, mIptvUnicastVlanAllMember, "IPTV Unicast");

    if (!result) {
        return false;

    }

    result = getUnicastVlanSettings(createOid(Mib::dot1qVlanStaticUntaggedPorts, 13, mParentDevice->iptvVlanTag()),
                                    14, mIptvUnicastVlanUntagMember, "IPTV Unicast");

    if (!result) {
        return false;
    }

    if ((mInetVlanAllMember.at(port - 1) == true)
            && (mInetVlanUntagMember.at(port - 1) == true)) {
        mError = QString::fromUtf8("Ошибка: Порт %1 уже прописан под Интернет.")
                 .arg(QString::number(port));

        return false;
    } else {
        mInetVlanAllMember.setBit(port - 1, true);
        mInetVlanUntagMember.setBit(port - 1, true);
        mIptvUnicastVlanAllMember.setBit(port - 1, false);
        mIptvUnicastVlanUntagMember.setBit(port - 1, false);
    }

    QVector<OidPair> oidList;
    oidList.push_back(OidPair(createOid(Mib::dot1qVlanStaticUntaggedPorts, 13, mParentDevice->iptvVlanTag()), 14));
    oidList.push_back(OidPair(createOid(Mib::dot1qVlanStaticEgressPorts, 13, mParentDevice->iptvVlanTag()), 14));
    oidList.push_back(OidPair(createOid(Mib::dot1qVlanStaticEgressPorts, 13, mParentDevice->inetVlanTag()), 14));
    oidList.push_back(OidPair(createOid(Mib::dot1qVlanStaticUntaggedPorts, 13, mParentDevice->inetVlanTag()), 14));

    QList<QBitArray> arrayList;
    arrayList.push_back(mIptvUnicastVlanUntagMember);
    arrayList.push_back(mIptvUnicastVlanAllMember);
    arrayList.push_back(mInetVlanAllMember);
    arrayList.push_back(mInetVlanUntagMember);

    return sendVlanSetting(oidList, arrayList, false);
}

bool SwitchPortListModel::setMemberInternetWithIptvStbService(int port)
{
    bool result = getUnicastVlanSettings(createOid(Mib::dot1qVlanStaticEgressPorts, 13, mParentDevice->inetVlanTag()),
                                         14, mInetVlanAllMember, "Inet");

    if (!result)
        return false;

    result = getUnicastVlanSettings(createOid(Mib::dot1qVlanStaticUntaggedPorts, 13, mParentDevice->inetVlanTag()),
                                    14, mInetVlanUntagMember, "Inet");

    if (!result)
        return false;

    result = getUnicastVlanSettings(createOid(Mib::dot1qVlanStaticEgressPorts, 13, mParentDevice->iptvVlanTag()),
                                    14, mIptvUnicastVlanAllMember, "IPTV Unicast");

    if (!result)
        return false;

    result = getUnicastVlanSettings(createOid(Mib::dot1qVlanStaticUntaggedPorts, 13, mParentDevice->iptvVlanTag()),
                                    14, mIptvUnicastVlanUntagMember, "IPTV Unicast");

    if (!result)
        return false;

    if ((mInetVlanAllMember[port - 1] == true)
            && (mInetVlanUntagMember[port - 1] == false)
            && (mIptvUnicastVlanAllMember[port - 1] == true)
            && (mIptvUnicastVlanUntagMember[port - 1] == true)) {
        mError = QString::fromUtf8("Ошибка: Порт %1 уже прописан под Интернет + IPTV STB.")
                 .arg(QString::number(port));

        return false;
    } else {
        mInetVlanAllMember.setBit(port - 1, true);
        mInetVlanUntagMember.setBit(port - 1, false);
        mIptvUnicastVlanAllMember.setBit(port - 1, true);
        mIptvUnicastVlanUntagMember.setBit(port - 1, true);
    }

    QVector<OidPair> oidList;
    oidList.push_back(OidPair(createOid(Mib::dot1qVlanStaticEgressPorts, 13, mParentDevice->inetVlanTag()), 14));
    oidList.push_back(OidPair(createOid(Mib::dot1qVlanStaticUntaggedPorts, 13, mParentDevice->inetVlanTag()), 14));
    oidList.push_back(OidPair(createOid(Mib::dot1qVlanStaticEgressPorts, 13, mParentDevice->iptvVlanTag()), 14));
    oidList.push_back(OidPair(createOid(Mib::dot1qVlanStaticUntaggedPorts, 13, mParentDevice->iptvVlanTag()), 14));

    QList<QBitArray> arrayList;
    arrayList.push_back(mInetVlanAllMember);
    arrayList.push_back(mInetVlanUntagMember);
    arrayList.push_back(mIptvUnicastVlanAllMember);
    arrayList.push_back(mIptvUnicastVlanUntagMember);

    return sendVlanSetting(oidList, arrayList, false);
}

bool SwitchPortListModel::updateInfoPort(int indexPort)
{
    QScopedPointer<SnmpClient> snmpClient(new SnmpClient());

    snmpClient->setIp(mParentDevice->ip());

    if (!snmpClient->setupSession(SessionType::ReadSession)) {
        mError = SnmpErrors::SetupSession;
        return false;
    }

    if (!snmpClient->openSession()) {
        mError = SnmpErrors::OpenSession;
        return false;
    }

    SwitchPortInfo::Ptr currentPort;

    auto it = mList.begin();
    auto end = mList.end();
    for (; it != end; ++it) {
        if ((*it)->number() == indexPort) {
            currentPort = *it;
            break;
        }
    }

    return updateInfoPort(snmpClient, currentPort);
}

bool SwitchPortListModel::updateInfoAllPort()
{
    getVlanSettings();

    QScopedPointer<SnmpClient> snmpClient(new SnmpClient());

    snmpClient->setIp(mParentDevice->ip());

    if (!snmpClient->setupSession(SessionType::ReadSession)) {
        mError = SnmpErrors::SetupSession;
        return false;
    }

    if (!snmpClient->openSession()) {
        mError = SnmpErrors::OpenSession;
        return false;
    }

    int count = mList.size();
    bool result = true;

    for (int i = 0; i < count; ++i) {
        result &= updateInfoPort(snmpClient, mList[i]);
    }

    if (!result)
        BasicDialogs::error(NULL, BasicDialogTitle::Error, QString::fromUtf8("При получении информации по портам произошли ошибки."));

    return true;
}

bool SwitchPortListModel::getVlanSettings()
{
    QString allErrors;

    bool result = getUnicastVlanSettings(createOid(Mib::dot1qVlanStaticEgressPorts, 13, mParentDevice->inetVlanTag()),
                                         14, mInetVlanAllMember, "Inet");



    result &= getUnicastVlanSettings(createOid(Mib::dot1qVlanStaticUntaggedPorts, 13, mParentDevice->inetVlanTag()),
                                    14, mInetVlanUntagMember, "Inet");

    if (!result)
        allErrors += mError + "\n";

    result = getUnicastVlanSettings(createOid(Mib::dot1qVlanStaticEgressPorts, 13, mParentDevice->iptvVlanTag()),
                                    14, mIptvUnicastVlanAllMember, "IPTV Unicast");

    result &= getUnicastVlanSettings(createOid(Mib::dot1qVlanStaticUntaggedPorts, 13, mParentDevice->iptvVlanTag()),
                                    14, mIptvUnicastVlanUntagMember, "IPTV Unicast");

    if (!result)
        allErrors += mError + "\n";

    result = getMulticastVlanSettings();

    if (!result)
        allErrors += mError + "\n";

    if (!allErrors.isEmpty())
        BasicDialogs::error(0, BasicDialogTitle::Error, QString::fromUtf8("Произоизошла ошибка при получении информации о принадлежности к сервисным VLAN!"), allErrors);

    return true;
}

bool SwitchPortListModel::updateInfoPort(QScopedPointer<SnmpClient> &snmpClient,
                                         SwitchPortInfo::Ptr updatingPort)
{
    int indexPort = updatingPort->number();

    snmpClient->createPdu(SNMP_MSG_GET);
    snmpClient->addOid(createOid(Mib::ifOperStatus, 10, indexPort), 11);

    if (mParentDevice->deviceModel() == DeviceModel::DES3526)
        snmpClient->addOid(createOid(Mib::swL2PortInfoNwayStatusDES3526, 15, indexPort), 16);
    else if (mParentDevice->deviceModel() == DeviceModel::DES3528) {
        long NumPort[] = {indexPort, 1};
        snmpClient->addOid(createOid(Mib::swL2PortInfoNwayStatusDES3528, 15, NumPort, 2, 1), 17);

        if (indexPort == 25 || indexPort == 26)
            snmpClient->addOid(createOid(Mib::swL2PortInfoNwayStatusDES3528, 15, NumPort, 2, 1), 17);
    } else if (mParentDevice->deviceModel() == DeviceModel::DES3550)
        snmpClient->addOid(createOid(Mib::swL2PortInfoNwayStatusDES3550, 15, indexPort), 16);
    else
        snmpClient->addOid(createOid(Mib::swL2PortInfoNwayStatusDES3526, 15, indexPort), 16);
    snmpClient->addOid(createOid(Mib::ifAlias, 11, indexPort), 12);

    if (snmpClient->sendRequest()) {
        netsnmp_variable_list *vars = snmpClient->varList();

        if (!isValidSnmpValue(vars))
            return false;

        updatingPort->setState(switchStatePortString(*vars->val.integer));
        emit dataChanged(index(indexPort - 1, 0), index(indexPort - 1, 0));

        vars = vars->next_variable;

        if (!isValidSnmpValue(vars))
            return false;

        if ((mParentDevice->deviceModel() == DeviceModel::DES3528) &&
                (indexPort == 25 || indexPort == 26)) {
            QString cupperState = speedDuplexString(mParentDevice->deviceModel(), *vars->val.integer);

            vars = vars->next_variable;

            if (!isValidSnmpValue(vars))
                return false;

            QString opticState = speedDuplexString(mParentDevice->deviceModel(), *(vars->val.integer));

            updatingPort->setSpeedDuplex(opticState % " | " % cupperState);
            emit dataChanged(index(indexPort - 1, 1), index(indexPort - 1, 1));
        } else {
            updatingPort->setSpeedDuplex(speedDuplexString(mParentDevice->deviceModel(), *(vars->val.integer)));
            emit dataChanged(index(indexPort - 1, 1), index(indexPort - 1, 1));
        }

        vars = vars->next_variable;

        if (!isValidSnmpValue(vars))
            return false;

        updatingPort->setDesc(QString::fromLatin1((char *)vars->val.string, vars->val_len));
        emit dataChanged(index(indexPort - 1, 2), index(indexPort - 1, 2));
    } else {
        mError = SnmpErrors::GetInfo;
        return false;
    }

    return true;
}

void SwitchPortListModel::createList()
{
    int count = countPorts(mParentDevice->deviceModel());

    mList.clear();

    for (int i = 0; i < count; ++i) {
        SwitchPortInfo::Ptr portInfo = SwitchPortInfo::Ptr(new SwitchPortInfo());
        mList.push_back(portInfo);
        mList[i]->setNumber(i + 1);
    }
}

bool SwitchPortListModel::getUnicastVlanSettings(const oid *oidVlan, int oidVlanLen, QBitArray &vlanPortArray, QString vlanName)
{
    QScopedPointer<SnmpClient> snmp(new SnmpClient());

    snmp->setIp(mParentDevice->ip());

    if (!snmp->setupSession(SessionType::ReadSession)) {
        mError = SnmpErrors::SetupSession;
        return false;
    }

    if (!snmp->openSession()) {
        mError = SnmpErrors::OpenSession;
        return false;
    }

    snmp->createPdu(SNMP_MSG_GET);

    oid *portsVlanOid = createOid(oidVlan, oidVlanLen);
    size_t portsVlanOidLen = oidVlanLen;

    snmp->addOid(oidVlan, oidVlanLen);

    bool result;

    if (snmp->sendRequest()) {
        netsnmp_variable_list *vars = snmp->varList();

        if ((vars->val_len < 4) ||
                (snmp_oid_ncompare(portsVlanOid, portsVlanOidLen, vars->name, vars->name_length, oidVlanLen) != 0)) {
            mError = QString::fromUtf8("Ошибка: Не удалось получить данные по влану %1.")
                     .arg(vlanName);
            return false;
        }

        vlanPortArray = ucharToQBitArray(mParentDevice->deviceModel(), vars->val.string);

        result = true;
    } else {
        mError = QString::fromUtf8("Ошибка: Не удалось получить данные по влану %1.")
                 .arg(vlanName);
        result = false;
    }

    return result;
}

bool SwitchPortListModel::getMulticastVlanSettings()
{
    QScopedPointer<SnmpClient> snmp(new SnmpClient());

    snmp->setIp(mParentDevice->ip());

    if (!snmp->setupSession(SessionType::ReadSession)) {
        mError = SnmpErrors::SetupSession;
        return false;
    }

    if (!snmp->openSession()) {
        mError = SnmpErrors::SetupSession;
        return false;
    }

    snmp->createPdu(SNMP_MSG_GETNEXT);

    size_t oidLen;

    if ((mParentDevice->deviceModel() == DeviceModel::DES3526)
            || (mParentDevice->deviceModel() == DeviceModel::DES3550))
        oidLen = 15;
    else if (mParentDevice->deviceModel() == DeviceModel::DES3528)
        oidLen = 13;
    else
        oidLen = 1; // левый случай которого не будет.

    oid *multicastVlanOid;
    size_t multicastVlanOidLen = oidLen;


    oid *nextOid;
    size_t nextOidLen = oidLen;


    if (mParentDevice->deviceModel() == DeviceModel::DES3526) {
        multicastVlanOid = createOid(Mib::swL2IGMPMulticastVlanMemberPortDES3526, oidLen);
        nextOid = createOid(Mib::swL2IGMPMulticastVlanMemberPortDES3526, oidLen);
    } else if (mParentDevice->deviceModel() == DeviceModel::DES3528) {
        multicastVlanOid = createOid(Mib::swL2IGMPMulticastVlanMemberPortDES3528, oidLen);
        nextOid = createOid(Mib::swL2IGMPMulticastVlanMemberPortDES3528, oidLen);
    } else if (mParentDevice->deviceModel() == DeviceModel::DES3550) {
        multicastVlanOid = createOid(Mib::swL2IGMPMulticastVlanMemberPortDES3550, oidLen);
        nextOid = createOid(Mib::swL2IGMPMulticastVlanMemberPortDES3550, oidLen);
    } else {
        return false;
    }


    snmp->addOid(nextOid, nextOidLen);

    bool result;

    if (snmp->sendRequest()) {
        netsnmp_variable_list *vars = snmp->varList();


        if ((vars->val_len < 4) ||
                (snmp_oid_ncompare(multicastVlanOid, multicastVlanOidLen, vars->name, vars->name_length, oidLen) != 0)) {
            mError = QString::fromUtf8("Ошибка: Не удалось получить данные по Multicast влану.");
            delete[] nextOid;
            delete[] multicastVlanOid;
            return false;
        }

        mIptvMultVlanTag = vars->name[vars->name_length - 1];
        mMulticastVlanMember = ucharToQBitArray(mParentDevice->deviceModel(), vars->val.string);

        result = true;
    } else {
        mError = QString::fromUtf8("Ошибка: Не удалось получить данные по Multicast влану.");
        result = false;
    }

    delete[] nextOid;
    delete[] multicastVlanOid;
    return result;
}

bool SwitchPortListModel::sendVlanSetting(QVector<OidPair> &oidList, QList<QBitArray> &arrayList, bool ismv)
{
    QScopedPointer<SnmpClient> snmp(new SnmpClient());

    snmp->setIp(mParentDevice->ip());

    if (!snmp->setupSession(SessionType::WriteSession)) {
        mError = SnmpErrors::SetupSession;
        return false;
    }

    if (!snmp->openSession()) {
        mError = SnmpErrors::OpenSession;
        return false;
    }

    snmp->createPdu(SNMP_MSG_SET);

    int size = arrayList.count();
    for (int i = 0; i < size; ++i) {
        QString arrayString = qbitArrayToHexString(mParentDevice->deviceModel(), arrayList[i], ismv);
        snmp->addOid(oidList[i].first, oidList[i].second, arrayString, 'x');
    }

    bool result = true;

    if (!snmp->sendRequest()) {
        mError = QString::fromUtf8("Ошибка: Не удалось изменить настройки порта!");
        result = false;
    }

    return result;
}
