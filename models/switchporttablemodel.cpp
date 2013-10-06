#include "switchporttablemodel.h"

#include <basicdialogs.h>
#include <constant.h>
#include <customsnmpfunctions.h>
#include <ports/switchportdes3526.h>
#include <ports/switchportdes3528.h>
#include <ports/switchportdes3550.h>

// Columns
// DisplayRole
// 0 - number
// 1 - operation state
// 2 - speed/duplex
// 3 - description

SwitchPortTableModel::SwitchPortTableModel(Switch::Ptr parentDevice, ImprovedMessageWidget *messageWidget,
                                           QObject *parent) :
    QAbstractTableModel(parent),
    mParentDevice(parentDevice),
    mFutureWatcher(new QFutureWatcher<void>()),
    mMessageWidget(messageWidget)
{
    createList();

    mIptvMultVlanTag = 0;

    int countBit = mParentDevice->sizePortBitArray();

    mInetVlanAllMember.fill(false, countBit);
    mInetVlanUntagMember.fill(false, countBit);
    mIptvUnicastVlanAllMember.fill(false, countBit);
    mIptvUnicastVlanUntagMember.fill(false, countBit);
    mMulticastVlanMember.fill(false, countBit);

    connect(mFutureWatcher, &QFutureWatcher<void>::finished,
            this, &SwitchPortTableModel::finishAsyncUpdate);
}

SwitchPortTableModel::~SwitchPortTableModel()
{
    if (mFutureWatcher) {
        if (mFutureWatcher->isRunning()) {
            mFutureWatcher->cancel();
            mFutureWatcher->waitForFinished();
        }

        delete mFutureWatcher;
    }
}

int SwitchPortTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return mList.size();
}

int SwitchPortTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 4;
}

QVariant SwitchPortTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole) {
        if (index.column() == NumberColumn) {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        } else {
            return int(Qt::AlignCenter | Qt::AlignVCenter);
        }
    } else if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (index.column() == NumberColumn) {
            return mList.at(index.row())->index();
        } else if (index.column() == StateColumn) {
            return SwitchPortState::toString(mList.at(index.row())->state());
        } else if (index.column() == SpeedDuplexColumn) {
            return mList.at(index.row())->speedDuplex();
        } else if (index.column() == DescColumn) {
            return mList.at(index.row())->description();
        }
    } else if (role == Qt::BackgroundColorRole) {
        if (mList.at(index.row())->state() == SwitchPortState::Down) {
            return QBrush(QColor(223, 255, 252));
        } else if (mList.at(index.row())->state() == SwitchPortState::Up) {
            return QBrush(QColor(200, 255, 200));
        }
    }

    return QVariant();
}

bool SwitchPortTableModel::setData(const QModelIndex &index, const QVariant &value,
                                   int role)
{
    if ((!index.isValid()) || (role != Qt::EditRole))
        return false;

    if (index.column() != DescColumn)
        return false;

    long portIndex = mList[index.row()]->index();

    if (value.toString() == mList[index.row()]->description())
        return true;

    bool result = mParentDevice->setPortDescription(portIndex, value.toString());

    if (result) {
        mList[index.row()]->setDescription(value.toString());
        QModelIndex descIndex = SwitchPortTableModel::index(portIndex, DescColumn);
        emit dataChanged(descIndex, descIndex);
    } else {
        mMessageWidget->setMessageType(ImprovedMessageWidget::Error);
        mMessageWidget->setText(mParentDevice->error());
        mMessageWidget->animatedShow();
    }

    return true;
}

QVariant SwitchPortTableModel::headerData(int section, Qt::Orientation orientation,
                                          int role) const
{
    if (orientation == Qt::Vertical)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (section) {
        case NumberColumn:
            return SwitchPortTableModelStrings::Number;
        case StateColumn:
            return SwitchPortTableModelStrings::State;
        case SpeedDuplexColumn:
            return SwitchPortTableModelStrings::SpeedDuplex;
        case DescColumn:
            return SwitchPortTableModelStrings::Desc;
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

Qt::ItemFlags SwitchPortTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if (index.column() == DescColumn)
        flags |= Qt::ItemIsEditable;

    return flags;
}

bool SwitchPortTableModel::memberMulticastVlan(int port)
{
    return mMulticastVlanMember.at(port - 1);
}

bool SwitchPortTableModel::setMemberMulticastVlan(int port, bool value)
{
    if (!getMulticastVlanSettings())
        return false;

    if (mMulticastVlanMember.at(port - 1) == value) {
        if (value) {
            mError = QString::fromUtf8("Порт %1 уже является членом Multicast влана.")
                     .arg(QString::number(port));
        } else {
            mError = QString::fromUtf8("Порт %1 не является членом Multicast влана.")
                     .arg(QString::number(port));
        }

        return false;
    } else {
        mMulticastVlanMember.setBit(port - 1, value);
    }

    QVector<OidPair> oidList;

    OidPair vlanMemberOid;
    if (mParentDevice->deviceModel() == DeviceModel::DES3526) {
        vlanMemberOid = createOidPair(Mib::swL2IGMPMulticastVlanMemberPortDES3526, 15, mIptvMultVlanTag);
    } else if (mParentDevice->deviceModel() == DeviceModel::DES3550) {
        vlanMemberOid = createOidPair(Mib::swL2IGMPMulticastVlanMemberPortDES3550, 15, mIptvMultVlanTag);
    } else {
        vlanMemberOid = createOidPair(Mib::swL2IGMPMulticastVlanMemberPortDES3528, 13, mIptvMultVlanTag);
    }

    oidList.push_back(vlanMemberOid);

    QList<QBitArray> arrayList;
    arrayList.push_back(mMulticastVlanMember);

    bool result = sendVlanSetting(oidList, arrayList, true);

    return result;
}

VlanState::Enum SwitchPortTableModel::memberInetVlan(int port)
{
    if (mInetVlanUntagMember.at(port - 1)) {
        return VlanState::Untag;
    } else if (mInetVlanAllMember.at(port - 1)) {
        return VlanState::Tag;
    } else {
        return VlanState::None;
    }
}

VlanState::Enum SwitchPortTableModel::memberIptvVlan(int port)
{
    if (mIptvUnicastVlanUntagMember.at(port - 1)) {
        return VlanState::Untag;
    } else if (mIptvUnicastVlanAllMember.at(port - 1)) {
        return VlanState::Tag;
    } else {
        return VlanState::None;
    }
}

bool SwitchPortTableModel::setMemberInternetService(int port)
{
    OidPair vlanMemberOid = createOidPair(Mib::dot1qVlanStaticEgressPorts, 13, mParentDevice->inetVlanTag());
    bool result = getUnicastVlanSettings(vlanMemberOid, mInetVlanAllMember, "Inet");

    if (!result)
        return false;

    vlanMemberOid = createOidPair(Mib::dot1qVlanStaticUntaggedPorts, 13, mParentDevice->inetVlanTag());
    result = getUnicastVlanSettings(vlanMemberOid, mInetVlanUntagMember, "Inet");

    if (!result)
        return false;

    vlanMemberOid = createOidPair(Mib::dot1qVlanStaticEgressPorts, 13, mParentDevice->iptvVlanTag());
    result = getUnicastVlanSettings(vlanMemberOid, mIptvUnicastVlanAllMember, "IPTV Unicast");

    if (!result)
        return false;

    vlanMemberOid = createOidPair(Mib::dot1qVlanStaticUntaggedPorts, 13, mParentDevice->iptvVlanTag());
    result = getUnicastVlanSettings(vlanMemberOid, mIptvUnicastVlanUntagMember, "IPTV Unicast");

    if (!result)
        return false;

    if ((mInetVlanAllMember.at(port - 1) == true)
            && (mInetVlanUntagMember.at(port - 1) == true)) {
        mError = QString::fromUtf8("Порт %1 уже прописан под Интернет.")
                 .arg(QString::number(port));

        return false;
    } else {
        mInetVlanAllMember.setBit(port - 1, true);
        mInetVlanUntagMember.setBit(port - 1, true);
        mIptvUnicastVlanAllMember.setBit(port - 1, false);
        mIptvUnicastVlanUntagMember.setBit(port - 1, false);
    }

    QVector<OidPair> oidList;
    oidList.push_back(createOidPair(Mib::dot1qVlanStaticUntaggedPorts, 13, mParentDevice->iptvVlanTag()));
    oidList.push_back(createOidPair(Mib::dot1qVlanStaticEgressPorts, 13, mParentDevice->iptvVlanTag()));
    oidList.push_back(createOidPair(Mib::dot1qVlanStaticEgressPorts, 13, mParentDevice->inetVlanTag()));
    oidList.push_back(createOidPair(Mib::dot1qVlanStaticUntaggedPorts, 13, mParentDevice->inetVlanTag()));

    QList<QBitArray> arrayList;
    arrayList.push_back(mIptvUnicastVlanUntagMember);
    arrayList.push_back(mIptvUnicastVlanAllMember);
    arrayList.push_back(mInetVlanAllMember);
    arrayList.push_back(mInetVlanUntagMember);

    return sendVlanSetting(oidList, arrayList, false);
}

bool SwitchPortTableModel::setMemberInternetWithIptvStbService(int port)
{
    OidPair vlanMemberOid = createOidPair(Mib::dot1qVlanStaticEgressPorts, 13, mParentDevice->inetVlanTag());
    bool result = getUnicastVlanSettings(vlanMemberOid, mInetVlanAllMember, "Inet");

    if (!result)
        return false;

    vlanMemberOid = createOidPair(Mib::dot1qVlanStaticUntaggedPorts, 13, mParentDevice->inetVlanTag());
    result = getUnicastVlanSettings(vlanMemberOid, mInetVlanUntagMember, "Inet");

    if (!result)
        return false;

    vlanMemberOid = createOidPair(Mib::dot1qVlanStaticEgressPorts, 13, mParentDevice->iptvVlanTag());
    result = getUnicastVlanSettings(vlanMemberOid, mIptvUnicastVlanAllMember, "IPTV Unicast");

    if (!result)
        return false;

    vlanMemberOid = createOidPair(Mib::dot1qVlanStaticUntaggedPorts, 13, mParentDevice->iptvVlanTag());
    result = getUnicastVlanSettings(vlanMemberOid, mIptvUnicastVlanUntagMember, "IPTV Unicast");

    if (!result)
        return false;

    if ((mInetVlanAllMember[port - 1] == true)
            && (mInetVlanUntagMember[port - 1] == false)
            && (mIptvUnicastVlanAllMember[port - 1] == true)
            && (mIptvUnicastVlanUntagMember[port - 1] == true)) {
        mError = QString::fromUtf8("Порт %1 уже прописан под Интернет + IPTV STB.")
                 .arg(QString::number(port));

        return false;
    } else {
        mInetVlanAllMember.setBit(port - 1, true);
        mInetVlanUntagMember.setBit(port - 1, false);
        mIptvUnicastVlanAllMember.setBit(port - 1, true);
        mIptvUnicastVlanUntagMember.setBit(port - 1, true);
    }

    QVector<OidPair> oidList;
    oidList.push_back(createOidPair(Mib::dot1qVlanStaticEgressPorts, 13, mParentDevice->inetVlanTag()));
    oidList.push_back(createOidPair(Mib::dot1qVlanStaticUntaggedPorts, 13, mParentDevice->inetVlanTag()));
    oidList.push_back(createOidPair(Mib::dot1qVlanStaticEgressPorts, 13, mParentDevice->iptvVlanTag()));
    oidList.push_back(createOidPair(Mib::dot1qVlanStaticUntaggedPorts, 13, mParentDevice->iptvVlanTag()));

    QList<QBitArray> arrayList;
    arrayList.push_back(mInetVlanAllMember);
    arrayList.push_back(mInetVlanUntagMember);
    arrayList.push_back(mIptvUnicastVlanAllMember);
    arrayList.push_back(mIptvUnicastVlanUntagMember);

    return sendVlanSetting(oidList, arrayList, false);
}

bool SwitchPortTableModel::updatePort(int index)
{
    mUpdateErrors.clear();

    bool result = updatePort(mList[index - 1]);

    if (!result)
        mError = mUpdateErrors;

    return result;
}

bool SwitchPortTableModel::updatePort(const SwitchPort::Ptr &port)
{
    QScopedPointer<SnmpClient> snmpClient(new SnmpClient());

    snmpClient->setIp(mParentDevice->ip());

    if (!snmpClient->setupSession(SessionType::ReadSession)) {
        appendUpdateError(QString::fromUtf8("Информацию по порту %1 получить не удалось. %2")
                          .arg(port->index())
                          .arg(SnmpErrorStrings::SetupSession));
        return false;
    }

    if (!snmpClient->openSession()) {
        appendUpdateError(QString::fromUtf8("Информацию по порту %1 получить не удалось. %2")
                          .arg(port->index())
                          .arg(SnmpErrorStrings::OpenSession));
        return false;
    }

    snmpClient->createPdu(SNMP_MSG_GET);

    port->fillPdu(snmpClient.data());

    if (snmpClient->sendRequest()) {
        if (!port->parsePdu(snmpClient.data())) {
            appendUpdateError(QString::fromUtf8("Информацию по порту %1 получить не удалось. %2")
                              .arg(port->index())
                              .arg(SnmpErrorStrings::Parse));
            return false;
        } else {
            emit dataChanged(index(port->index() - 1, 1), index(port->index() - 1, 3));
        }
    } else {
        appendUpdateError(QString::fromUtf8("Информацию по порту %1 получить не удалось. %2")
                          .arg(port->index())
                          .arg(snmpClient->error()));
        return false;
    }

    return true;
}

void SwitchPortTableModel::update()
{
    mUpdateErrors.clear();

    if (mFutureWatcher->isRunning()) {
        mError = QString::fromUtf8("Обновление информации по портам уже выполняется.");
        emit updateFinished(true);
        return;
    }

    QFuture<void> future = QtConcurrent::map(mList, SwitchUpdateWrapperObject(this));
    mFutureWatcher->setFuture(future);

    getVlanSettings();
}

bool SwitchPortTableModel::updateIsRunning()
{
    return mFutureWatcher->isRunning();
}

bool SwitchPortTableModel::getVlanSettings()
{
    QString allErrors;

    OidPair vlanMemberOid = createOidPair(Mib::dot1qVlanStaticEgressPorts, 13, mParentDevice->inetVlanTag());
    bool result = getUnicastVlanSettings(vlanMemberOid, mInetVlanAllMember, "Inet");

    vlanMemberOid = createOidPair(Mib::dot1qVlanStaticUntaggedPorts, 13, mParentDevice->inetVlanTag());
    result &= getUnicastVlanSettings(vlanMemberOid, mInetVlanUntagMember, "Inet");

    if (!result)
        allErrors += mError + "\n";

    vlanMemberOid = createOidPair(Mib::dot1qVlanStaticEgressPorts, 13, mParentDevice->iptvVlanTag());
    result = getUnicastVlanSettings(vlanMemberOid, mIptvUnicastVlanAllMember, "IPTV Unicast");

    vlanMemberOid = createOidPair(Mib::dot1qVlanStaticUntaggedPorts, 13, mParentDevice->iptvVlanTag());
    result &= getUnicastVlanSettings(vlanMemberOid, mIptvUnicastVlanUntagMember, "IPTV Unicast");

    if (!result)
        allErrors += mError + "\n";

    result = getMulticastVlanSettings();

    if (!result)
        allErrors += mError + "\n";

    if (!allErrors.isEmpty()) {
        mMessageWidget->setMessageType(ImprovedMessageWidget::Error);
        mMessageWidget->setText(trUtf8("Произошла ошибка при получении информации о принадлежности к сервисным VLAN!"), allErrors);
        mMessageWidget->animatedShow();
    }

    return true;
}

QString SwitchPortTableModel::error() const
{
    return mError;
}

void SwitchPortTableModel::createList()
{
    int count = mParentDevice->countPorts();
    mList.reserve(count);

    for (int i = 1; i <= count; ++i) {
        SwitchPort::Ptr portInfo = mParentDevice->createPort(this);

        if (!portInfo)
            return;

        portInfo->setIndex(i);

        mList.push_back(portInfo);
    }
}

bool SwitchPortTableModel::getUnicastVlanSettings(const OidPair &oidVlan,
                                                  QBitArray &vlanPortArray,
                                                  QString vlanName)
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

    snmp->createPdu(SNMP_MSG_GET);

    snmp->addOid(oidVlan);

    bool result;

    if (snmp->sendRequest()) {
        netsnmp_variable_list *vars = snmp->varList();

        if ((vars->val_len < 4) ||
                (snmp_oid_ncompare(oidVlan.first, oidVlan.second,
                                   vars->name, vars->name_length,
                                   oidVlan.second) != 0)) {
            mError = QString::fromUtf8("Не удалось получить данные по влану %1.")
                     .arg(vlanName);

            return false;
        }

        vlanPortArray = ucharToQBitArray(mParentDevice->deviceModel(),
                                         vars->val.string);

        result = true;
    } else {
        mError = QString::fromUtf8("Не удалось получить данные по влану %1.")
                 .arg(vlanName);
        result = false;
    }

    return result;
}

bool SwitchPortTableModel::getMulticastVlanSettings()
{
    QScopedPointer<SnmpClient> snmpClient(new SnmpClient());

    snmpClient->setIp(mParentDevice->ip());

    if (!snmpClient->setupSession(SessionType::ReadSession)) {
        mError = SnmpErrorStrings::SetupSession;
        return false;
    }

    if (!snmpClient->openSession()) {
        mError = SnmpErrorStrings::SetupSession;
        return false;
    }

    snmpClient->createPdu(SNMP_MSG_GETNEXT);

    OidPair mvrMemberOid;
    if (mParentDevice->deviceModel() == DeviceModel::DES3526) {
        mvrMemberOid = createOidPair(Mib::swL2IGMPMulticastVlanMemberPortDES3526, 15);
    } else if (mParentDevice->deviceModel() == DeviceModel::DES3528) {
        mvrMemberOid = createOidPair(Mib::swL2IGMPMulticastVlanMemberPortDES3528, 13);
    } else if (mParentDevice->deviceModel() == DeviceModel::DES3550) {
        mvrMemberOid = createOidPair(Mib::swL2IGMPMulticastVlanMemberPortDES3550, 15);
    } else {
        return false;
    }

    snmpClient->addOid(mvrMemberOid);

    if (snmpClient->sendRequest()) {
        netsnmp_variable_list *vars = snmpClient->varList();

        if ((vars->val_len < 4) ||
                (snmp_oid_ncompare(mvrMemberOid.first, mvrMemberOid.second,
                                   vars->name, vars->name_length,
                                   mvrMemberOid.second) != 0)) {
            mError = QString::fromUtf8("Не удалось получить данные по Multicast влану.");

            return false;
        }

        mIptvMultVlanTag = vars->name[vars->name_length - 1];
        mMulticastVlanMember = ucharToQBitArray(mParentDevice->deviceModel(), vars->val.string);

        return true;
    } else {
        mError = QString::fromUtf8("Не удалось получить данные по Multicast влану.");

        return false;
    }
}

bool SwitchPortTableModel::sendVlanSetting(QVector<OidPair> &oidList,
                                           QList<QBitArray> &arrayList, bool ismv)
{
    QScopedPointer<SnmpClient> snmpClient(new SnmpClient());

    snmpClient->setIp(mParentDevice->ip());

    if (!snmpClient->setupSession(SessionType::WriteSession)) {
        mError = SnmpErrorStrings::SetupSession;
        return false;
    }

    if (!snmpClient->openSession()) {
        mError = SnmpErrorStrings::OpenSession;
        return false;
    }

    snmpClient->createPdu(SNMP_MSG_SET);

    int size = arrayList.size();
    for (int i = 0; i < size; ++i) {
        QString arrayString = qbitArrayToHexString(mParentDevice->deviceModel(),
                                                   arrayList[i], ismv);
        snmpClient->addOid(oidList.at(i), arrayString, 'x');
    }

    bool result = true;

    if (!snmpClient->sendRequest()) {
        mError = QString::fromUtf8("Не удалось изменить настройки порта!");
        result = false;
    }

    return result;
}

void SwitchPortTableModel::finishAsyncUpdate()
{
    if (!mUpdateErrors.isEmpty())
        mError = mUpdateErrors;

    emit updateFinished(!mUpdateErrors.isEmpty());
}

QBitArray SwitchPortTableModel::ucharToQBitArray(DeviceModel::Enum deviceModel, uchar *str)
{
    int countBits = 0;

    if ((deviceModel == DeviceModel::DES3526)
            || (deviceModel == DeviceModel::DES3528)) {
        countBits = 32;
    } else if (deviceModel == DeviceModel::DES3550) {
        countBits = 56;
    }

    QBitArray array(countBits, false);

    int firstBit = 0;
    int countBytes = countBits / 8;

    for (int i = 0; i < countBytes; ++i) {
        quint8 byteElement = str[i];

        if (byteElement == 0) {
            //так как массив уже инициализирован нулями
            //если текущий байт равен 0, итерация просто пропускается
            firstBit += 8;
            continue;
        }

        for (int bits = 0; bits < 8; ++bits) {
            array[7 - bits + firstBit] = byteElement % 2 == 1 ? true : false;

            byteElement /= 2;
        }

        firstBit += 8;
    }

    return array;
}

QString SwitchPortTableModel::qbitArrayToHexString(DeviceModel::Enum deviceModel, const QBitArray &array, bool ismv)
{
    QString tempStr = "";

    quint8 value = 0;

    int size = array.size();
    for (int i = 0; i < size; ++i) {
        value <<= 1;
        value += (quint8)array.at(i);

        if ((i + 1) % 8 == 0) { //проверка на последний bit в текущем байте
            tempStr.push_back(QString(" %1").arg(value, 2, 16, QLatin1Char('0')));
            value = 0;
        }
    }

    if ((deviceModel == DeviceModel::DES3528) && ismv) {
        // Значение MembersISMV в DES-3528 имеет размер 64 байта
        // Дополняем HEX-строку до 64 байт.
        for (int i = 0; i < 60; ++i)
            tempStr.push_back(QString(" 00"));
    }

    return tempStr.trimmed().toUpper();
}
