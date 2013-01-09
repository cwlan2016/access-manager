#include "switchportlistmodel.h"



// Columns
// DisplayRole
// 0 - number
// 1 - operation state
// 2 - speed/duplex
// 3 - description

SwitchPortListModel::SwitchPortListModel(DeviceModel deviceModel, QObject* parent) :
    QAbstractTableModel(parent)
{
    mDeviceModel = deviceModel;

    createList();

    mInetVlanTag = mIptvVlanTag = mIptvMultVlanTag = 0;

    int countBit = 0;

    if ((mDeviceModel == DeviceModel::DES3526)
            || (mDeviceModel == DeviceModel::DES3528))
        countBit = 32;
    else if (mDeviceModel == DeviceModel::DES3550)
        countBit = 56;
    else
        countBit = 32;

    mMulticastVlanMember.fill(false, countBit);
    mInetVlanAllMember.fill(false, countBit);
    mInetVlanUntagMember.fill(false, countBit);
    mIptvUnicastVlanAllMember.fill(false, countBit);
    mIptvUnicastVlanUntagMember.fill(false, countBit);
}

int SwitchPortListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return mPortList.size();
}

int SwitchPortListModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return 4;
}

QVariant SwitchPortListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole)
    {
        if (index.column() == 0)
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        else
            return int(Qt::AlignCenter | Qt::AlignVCenter);
    }
    else if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if (index.column() == 0)
            return mPortList[index.row()]->number();
        else if (index.column() == 1)
            return mPortList[index.row()]->state();
        else if (index.column() == 2)
            return mPortList[index.row()]->speedDuplex();
        else if (index.column() == 3)
            return mPortList[index.row()]->desc();
        else
            return QVariant();
    }
    else if ((role == Qt::BackgroundColorRole) && (index.internalId() == 0))
    {
        if (mPortList[index.row()]->state() == "Down")
        {
            return QBrush(QColor(223, 255, 252));
        }
        else if (mPortList[index.row()]->state() == "Up")
        {
            return QBrush(QColor(200, 255, 200));
        }
        else
            return QVariant();
    }
    else
        return QVariant();
}

QVariant SwitchPortListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        if(section == 0)
            return SwitchPortListModelColumn::Number;
        else if(section == 1)
            return SwitchPortListModelColumn::State;
        else if(section == 2)
            return SwitchPortListModelColumn::SpeedDuplex;
        else if(section == 3)
            return SwitchPortListModelColumn::Desc;
        else
            return QVariant();
    }
    else if (role == Qt::TextAlignmentRole)
    {
        return int(Qt::AlignCenter | Qt::AlignVCenter);
    }
    else if (role == Qt::FontRole)
    {
        QFont font(qApp->font());
        font.setBold(true);
        return font;
    }

    return QVariant();
}

void SwitchPortListModel::sort(int column, Qt::SortOrder order)
{
    std::sort(mPortList.begin(), mPortList.end(),
              [&](const SwitchPortInfo::Ptr& first,
                  const SwitchPortInfo::Ptr& second)
              {
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

    reset();
}

Qt::ItemFlags SwitchPortListModel::flags(const QModelIndex& index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

DeviceModel SwitchPortListModel::deviceModel()
{
    return mDeviceModel;
}

void SwitchPortListModel::setIP(QString ip)
{
    mIp = ip;
}

void SwitchPortListModel::setInetVlanTag(uint vlanTag)
{
    mInetVlanTag = vlanTag;
}

void SwitchPortListModel::setIptvVlanTag(uint vlanTag)
{
    mIptvVlanTag = vlanTag;
}

QString SwitchPortListModel::ip()
{
    return mIp;
}

QString SwitchPortListModel::error() const
{
    return mError;
}

bool SwitchPortListModel::memberMulticastVlan(int port)
{
    return mMulticastVlanMember[port - 1];
}

VlanState SwitchPortListModel::memberInetVlan(int port)
{
    if (mInetVlanUntagMember[port - 1])
    {
        return VlanState::Untag;
    }
    else if (mInetVlanAllMember[port - 1])
    {
        return VlanState::Tag;
    }
    else
    {
        return VlanState::None;
    }
}

VlanState SwitchPortListModel::memberIptvVlan(int port)
{
    if (mIptvUnicastVlanUntagMember[port - 1])
    {
        return VlanState::Untag;
    }
    else if (mIptvUnicastVlanAllMember[port - 1])
    {
        return VlanState::Tag;
    }
    else
    {
        return VlanState::None;
    }
}

bool SwitchPortListModel::setMemberMulticastVlan(int port, bool value)
{
    if (!getMulticastVlanSettings())
    {
        return false;
    }

    if (mMulticastVlanMember[port - 1] == value)
    {
        if (value)
            mError = QString::fromUtf8("Ошибка: Порт %1 уже является членом Multicast влана.")
                      .arg(QString::number(port));
        else
            mError = QString::fromUtf8("Ошибка: Порт %1 не является членом Multicast влана.")
                      .arg(QString::number(port));

        return false;
    }
    else
    {
        mMulticastVlanMember.setBit(port - 1, value);
    }

    QVector<OidPair> oidList;

    if (mDeviceModel == DeviceModel::DES3526)
//        oidStringList << Mib::swL2IGMPMulticastVlanMemberPortDES3526 % QString::number(mIptvMultVlanTag);
        oidList.push_back(OidPair(CreateOid(Mib::swL2IGMPMulticastVlanMemberPortDES3526, 15, mIptvMultVlanTag), 15));
    else if (mDeviceModel == DeviceModel::DES3550)
//        oidStringList << Mib::swL2IGMPMulticastVlanMemberPortDES3550 % QString::number(mIptvMultVlanTag);
        oidList.push_back(OidPair(CreateOid(Mib::swL2IGMPMulticastVlanMemberPortDES3550, 15, mIptvMultVlanTag), 15));
    else
//        oidStringList << Mib::swL2IGMPMulticastVlanMemberPortDES3528 % QString::number(mIptvMultVlanTag);
        oidList.push_back(OidPair(CreateOid(Mib::swL2IGMPMulticastVlanMemberPortDES3528, 13, mIptvMultVlanTag), 13));

    QList<QBitArray> arrayList;
    arrayList.append(mMulticastVlanMember);
    return sendVlanSetting(oidList, arrayList, true);
}

bool SwitchPortListModel::setMemberInternetService(int port)
{
    //bool result = getUnicastVlanSettings(Mib::dot1qVlanStaticEgressPorts % QString::number(mInetVlanTag), mInetVlanAllMember, "Inet");
    bool result = getUnicastVlanSettings(CreateOid(Mib::dot1qVlanStaticEgressPorts, 13, mInetVlanTag),
                                         13, mInetVlanAllMember, "Inet");

    if (!result)
    {
        return false;
    }

    result = getUnicastVlanSettings(CreateOid(Mib::dot1qVlanStaticUntaggedPorts, 13, mInetVlanTag),
                                    13, mInetVlanUntagMember, "Inet");

    if (!result)
    {
        return false;
    }

    result = getUnicastVlanSettings(CreateOid(Mib::dot1qVlanStaticEgressPorts, 13, mIptvVlanTag),
                                    13, mIptvUnicastVlanAllMember, "IPTV Unicast");

    if (!result)
    {
        return false;

    }

    result = getUnicastVlanSettings(CreateOid(Mib::dot1qVlanStaticUntaggedPorts, 13, mIptvVlanTag),
                                    13, mIptvUnicastVlanUntagMember, "IPTV Unicast");

    if (!result)
    {
        return false;
    }

    if ((mInetVlanAllMember[port - 1] == true)
            && (mInetVlanUntagMember[port - 1] == true))
    {
        mError = QString::fromUtf8("Ошибка: Порт %1 уже прописан под Интернет.")
                  .arg(QString::number(port));

        return false;
    }
    else
    {
        mInetVlanAllMember.setBit(port - 1, true);
        mInetVlanUntagMember.setBit(port - 1, true);
        mIptvUnicastVlanAllMember.setBit(port - 1, false);
        mIptvUnicastVlanUntagMember.setBit(port - 1, false);
    }

    QVector<OidPair> oidList;
    oidList.push_back(OidPair(CreateOid(Mib::dot1qVlanStaticUntaggedPorts, 13, mIptvVlanTag), 13));
    oidList.push_back(OidPair(CreateOid(Mib::dot1qVlanStaticEgressPorts, 13, mIptvVlanTag), 13));
    oidList.push_back(OidPair(CreateOid(Mib::dot1qVlanStaticEgressPorts, 13, mInetVlanTag), 13));
    oidList.push_back(OidPair(CreateOid(Mib::dot1qVlanStaticUntaggedPorts, 13, mInetVlanTag), 13));

    QList<QBitArray> arrayList;
    arrayList.push_back(mIptvUnicastVlanUntagMember);
    arrayList.push_back(mIptvUnicastVlanAllMember);
    arrayList.push_back(mInetVlanAllMember);
    arrayList.push_back(mInetVlanUntagMember);

    return sendVlanSetting(oidList, arrayList, false);
}

bool SwitchPortListModel::setMemberInternetWithIptvStbService(int port)
{
    bool result = getUnicastVlanSettings(CreateOid(Mib::dot1qVlanStaticEgressPorts, 13, mInetVlanTag),
                                         13, mInetVlanAllMember, "Inet");

    if (!result)
    {
        return false;
    }

    result = getUnicastVlanSettings(CreateOid(Mib::dot1qVlanStaticUntaggedPorts, 13, mInetVlanTag),
                                    13, mInetVlanUntagMember, "Inet");

    if (!result)
    {
        return false;
    }

    result = getUnicastVlanSettings(CreateOid(Mib::dot1qVlanStaticEgressPorts, 13, mIptvVlanTag),
                                    13, mIptvUnicastVlanAllMember, "IPTV Unicast");

    if (!result)
    {
        return false;

    }

    result = getUnicastVlanSettings(CreateOid(Mib::dot1qVlanStaticUntaggedPorts, 13, mIptvVlanTag),
                                    13, mIptvUnicastVlanUntagMember, "IPTV Unicast");

    if (!result)
    {
        return false;
    }

    if ((mInetVlanAllMember[port - 1] == true)
            && (mInetVlanUntagMember[port - 1] == false)
            && (mIptvUnicastVlanAllMember[port - 1] == true)
            && (mIptvUnicastVlanUntagMember[port - 1] == true))
    {
        mError = QString::fromUtf8("Ошибка: Порт %1 уже прописан под Интернет + IPTV STB.")
                  .arg(QString::number(port));

        return false;
    }
    else
    {
        mInetVlanAllMember.setBit(port - 1, true);
        mInetVlanUntagMember.setBit(port - 1, false);
        mIptvUnicastVlanAllMember.setBit(port - 1, true);
        mIptvUnicastVlanUntagMember.setBit(port - 1, true);
    }

    QVector<OidPair> oidList;
    oidList.push_back(OidPair(CreateOid(Mib::dot1qVlanStaticEgressPorts, 13, mInetVlanTag), 13));
    oidList.push_back(OidPair(CreateOid(Mib::dot1qVlanStaticUntaggedPorts, 13, mInetVlanTag), 13));
    oidList.push_back(OidPair(CreateOid(Mib::dot1qVlanStaticEgressPorts, 13, mIptvVlanTag), 13));
    oidList.push_back(OidPair(CreateOid(Mib::dot1qVlanStaticUntaggedPorts, 13, mIptvVlanTag), 13));

    QList<QBitArray> arrayList;
    arrayList.push_back(mInetVlanAllMember);
    arrayList.push_back(mInetVlanUntagMember);
    arrayList.push_back(mIptvUnicastVlanAllMember);
    arrayList.push_back(mIptvUnicastVlanUntagMember);

    return sendVlanSetting(oidList, arrayList, false);
}

bool SwitchPortListModel::updateInfoPort(int indexPort)
{
    QScopedPointer<SnmpClient> snmp(new SnmpClient());

    snmp->setIP(mIp);

    if (!snmp->setupSession(SessionType::ReadSession))
    {
        mError = SnmpErrors::SetupSession;
        return false;
    }

    if (!snmp->openSession())
    {
        mError = SnmpErrors::OpenSession;
        return false;
    }

    snmp->createPdu(SNMP_MSG_GET);

    snmp->addOid(CreateOid(Mib::ifOperStatus, 11, indexPort), 11);

    if (mDeviceModel == DeviceModel::DES3526)
        snmp->addOid(CreateOid(Mib::swL2PortInfoNwayStatusDES3526, 16, indexPort), 16);
    else if (mDeviceModel == DeviceModel::DES3528)
    {
//        snmp->addOid(Mib::swL2PortInfoNwayStatusDES3528 % QString::number(indexPort) % ".1");
        long NumPort[] = {indexPort, 1};
        snmp->addOid(CreateOid(Mib::swL2PortInfoNwayStatusDES3528, 15, NumPort, 2,1), 17);

        if(indexPort == 25 || indexPort == 26)
//            snmp->addOid(Mib::swL2PortInfoNwayStatusDES3528 % QString::number(indexPort) % ".2");

            snmp->addOid(CreateOid(Mib::swL2PortInfoNwayStatusDES3528, 15, NumPort,2, 2), 17);
    }
    else if (mDeviceModel == DeviceModel::DES3550)
//        snmp->addOid(Mib::swL2PortInfoNwayStatusDES3550 % QString::number(indexPort));
        snmp->addOid(CreateOid(Mib::swL2PortInfoNwayStatusDES3550, 15, indexPort), 16);
    else
        snmp->addOid(CreateOid(Mib::swL2PortInfoNwayStatusDES3526, 15, indexPort), 16);
    snmp->addOid(CreateOid(Mib::ifAlias, 12, indexPort), 12);

    if (snmp->sendRequest())
    {
        netsnmp_variable_list* vars = snmp->varList();

        if (!IsValidSnmpValue(vars))
            return false;

        SwitchPortInfo::Ptr currentPort;

        for (auto item : mPortList)
        {
            if (item->number() == indexPort)
            {
                currentPort = item;
                break;
            }
        }

        currentPort->setState(SwitchStatePortString(*vars->val.integer));
        emit dataChanged(index(indexPort - 1, 0), index(indexPort - 1, 0));

        vars = vars->next_variable;

        if (!IsValidSnmpValue(vars))
            return false;

        if ((mDeviceModel == DeviceModel::DES3528) &&
                (indexPort == 25 || indexPort == 26))
        {
            QString cupperState = SpeedDuplexString(mDeviceModel, *vars->val.integer);

            vars = vars->next_variable;

            if (!IsValidSnmpValue(vars))
                return false;

            QString opticState = SpeedDuplexString(mDeviceModel, *(vars->val.integer));

            currentPort->setSpeedDuplex(opticState % " | " % cupperState);
            emit dataChanged(index(indexPort - 1, 1), index(indexPort - 1, 1));
        }
        else
        {
            currentPort->setSpeedDuplex(SpeedDuplexString(mDeviceModel, *(vars->val.integer)));
            emit dataChanged(index(indexPort - 1, 1), index(indexPort - 1, 1));
        }

        vars = vars->next_variable;

        if (!IsValidSnmpValue(vars))
            return false;

        currentPort->setDesc(QString::fromLatin1((char*)vars->val.string, vars->val_len));
        emit dataChanged(index(indexPort - 1, 2), index(indexPort - 1, 2));
    }
    else
    {
        mError = SnmpErrors::GetInfo;
        return false;
    }

    return true;
}

bool SwitchPortListModel::updateInfoAllPort()
{
    getVlanSettings();


    int count = CountPorts(mDeviceModel);

    bool result = true;

    for (int i = 0; i < count; i++)
    {
        result &= updateInfoPort(i + 1);
    }

    if (!result)
        BasicDialogs::error(NULL, BasicDialogTitle::Error, QString::fromUtf8("При получении информации по портам произошли ошибки. Подробности в логах."));

    return true;
}

bool SwitchPortListModel::getVlanSettings()
{
    bool result = getUnicastVlanSettings(CreateOid(Mib::dot1qVlanStaticEgressPorts, 13, mInetVlanTag),
                                         13, mInetVlanAllMember, "Inet");

    if (!result)
    {
        BasicDialogs::error(NULL, BasicDialogTitle::Error, mError);
    }

    result = getUnicastVlanSettings(CreateOid(Mib::dot1qVlanStaticUntaggedPorts, 13, mInetVlanTag),
                                    13, mInetVlanUntagMember, "Inet");

    if (!result)
    {
        BasicDialogs::error(NULL, BasicDialogTitle::Error, mError);
    }

    result = getUnicastVlanSettings(CreateOid(Mib::dot1qVlanStaticEgressPorts, 13, mIptvVlanTag),
                                    13, mIptvUnicastVlanAllMember, "IPTV Unicast");

    if (!result)
    {
        BasicDialogs::error(NULL, BasicDialogTitle::Error, mError);
    }

    result = getUnicastVlanSettings(CreateOid(Mib::dot1qVlanStaticUntaggedPorts, 13, mIptvVlanTag),
                                    13, mIptvUnicastVlanUntagMember, "IPTV Unicast");

    if (!result)
    {
        BasicDialogs::error(NULL, BasicDialogTitle::Error, mError);
    }

    result = getMulticastVlanSettings();

    if (!result)
    {
        BasicDialogs::error(NULL, BasicDialogTitle::Error, mError);
    }

    return true;
}

void SwitchPortListModel::createList()
{
    int count = CountPorts(mDeviceModel);

    mPortList.clear();

    for (int i = 0; i < count; i++)
    {
        SwitchPortInfo::Ptr portInfo = std::make_shared<SwitchPortInfo>();
        mPortList.push_back(std::move(portInfo));
        mPortList[i]->setNumber(i + 1);
    }
}

bool SwitchPortListModel::getUnicastVlanSettings(const oid *oidVlan, int oidVlanLen, QBitArray &vlanPortArray, QString vlanName)
{
    QScopedPointer<SnmpClient> snmp(new SnmpClient());

    snmp->setIP(mIp);

    if (!snmp->setupSession(SessionType::ReadSession))
    {
        mError = SnmpErrors::SetupSession;
        return false;
    }

    if (!snmp->openSession())
    {
        mError = SnmpErrors::OpenSession;
        return false;
    }

    snmp->createPdu(SNMP_MSG_GET);

    //oid portsVlanOid[14];
    oid *portsVlanOid = CreateOid(oidVlan, oidVlanLen);
    size_t portsVlanOid_len = oidVlanLen;
    //snmp_parse_oid(oidString.toLatin1().data(), portsVlanOid, &portsVlanOid_len);

    snmp->addOid(oidVlan, oidVlanLen);

    bool result;

    if (snmp->sendRequest())
    {
        netsnmp_variable_list* vars = snmp->varList();

        if ((vars->val_len < 4) ||
                (snmp_oid_ncompare(portsVlanOid, portsVlanOid_len, vars->name, vars->name_length, 14) != 0))
        {
            mError = QString::fromUtf8("Ошибка: Не удалось получить данные по влану %1.")
                      .arg(vlanName);
            return false;
        }

        vlanPortArray = UCharToQBitArray(mDeviceModel, vars->val.string);

        result = true;
    }
    else
    {
        mError = QString::fromUtf8("Ошибка: Не удалось получить данные по влану %1.")
                  .arg(vlanName);
        result = false;
    }

    return result;
}

bool SwitchPortListModel::getMulticastVlanSettings()
{
    QScopedPointer<SnmpClient> snmp(new SnmpClient());

    snmp->setIP(mIp);

    if (!snmp->setupSession(SessionType::ReadSession))
    {
        mError = SnmpErrors::SetupSession;
        return false;
    }

    if (!snmp->openSession())
    {
        mError = SnmpErrors::SetupSession;
        return false;
    }

    snmp->createPdu(SNMP_MSG_GETNEXT);

    size_t oidLen;

    if ((mDeviceModel == DeviceModel::DES3526)
            || (mDeviceModel == DeviceModel::DES3550))
        oidLen = 15;
    else if (mDeviceModel == DeviceModel::DES3528)
        oidLen = 13;
    else
        oidLen = 1; // левый случай которого не будет.

    oid* multicastVlanOid;// = new oid[oidLen];
    size_t multicastVlanOidLen = oidLen;


    oid* nextOid;// = new oid[oidLen];
    size_t nextOidLen = oidLen;


    if (mDeviceModel == DeviceModel::DES3526)
    {
        multicastVlanOid = CreateOid(Mib::swL2IGMPMulticastVlanMemberPortDES3526, oidLen);
        nextOid = CreateOid(Mib::swL2IGMPMulticastVlanMemberPortDES3526, nextOidLen);
        //snmp_parse_oid(Mib::swL2IGMPMulticastVlanMemberPortDES3526.toLatin1(), multicastVlanOid, &multicastVlanOidLen);
        //snmp_parse_oid(Mib::swL2IGMPMulticastVlanMemberPortDES3526.toLatin1(), nextOid, &nextOidLen);
    }
    else if (mDeviceModel == DeviceModel::DES3528)
    {
        multicastVlanOid = CreateOid(Mib::swL2IGMPMulticastVlanMemberPortDES3528, oidLen);
        nextOid = CreateOid(Mib::swL2IGMPMulticastVlanMemberPortDES3528, nextOidLen);
        //snmp_parse_oid(Mib::swL2IGMPMulticastVlanMemberPortDES3528.toLatin1(), multicastVlanOid, &multicastVlanOidLen);
        //snmp_parse_oid(Mib::swL2IGMPMulticastVlanMemberPortDES3528.toLatin1(), nextOid, &nextOidLen);
    }
    else if (mDeviceModel == DeviceModel::DES3550)
    {
        multicastVlanOid = CreateOid(Mib::swL2IGMPMulticastVlanMemberPortDES3550, oidLen);
        nextOid = CreateOid(Mib::swL2IGMPMulticastVlanMemberPortDES3550, nextOidLen);
        //snmp_parse_oid(Mib::swL2IGMPMulticastVlanMemberPortDES3550.toLatin1(), multicastVlanOid, &multicastVlanOidLen);
        //snmp_parse_oid(Mib::swL2IGMPMulticastVlanMemberPortDES3550.toLatin1(), nextOid, &nextOidLen);
    }

    snmp->addOid(nextOid, nextOidLen);

    bool result;

    if (snmp->sendRequest())
    {
        netsnmp_variable_list* vars = snmp->varList();


        if ((vars->val_len < 4) ||
                (snmp_oid_ncompare(multicastVlanOid, multicastVlanOidLen, vars->name, vars->name_length, oidLen) != 0))
        {
            mError = QString::fromUtf8("Ошибка: Не удалось получить данные по Multicast влану.");
            delete[] nextOid;
            delete[] multicastVlanOid;
            return false;
        }

        mIptvMultVlanTag = vars->name[vars->name_length - 1];
        mMulticastVlanMember = UCharToQBitArray(mDeviceModel, vars->val.string);

        result = true;
    }
    else
    {
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

    snmp->setIP(mIp);

    if (!snmp->setupSession(SessionType::WriteSession))
    {
        mError = SnmpErrors::SetupSession;
        return false;
    }

    if (!snmp->openSession())
    {
        mError = SnmpErrors::OpenSession;
        return false;
    }

    snmp->createPdu(SNMP_MSG_SET);

    for (int i = 0; i < arrayList.count(); i++)
    {
        QString arrayString = QBitArrayToHexString(mDeviceModel, arrayList[i], ismv);
        snmp->addOid(oidList[i].first, oidList[i].second, arrayString, 'x');
    }

    bool result = true;

    if (!snmp->sendRequest())
    {
        mError = QString::fromUtf8("Ошибка: Не удалось изменить настройки порта!");
        result = false;
    }

    return result;
}
