#include "dslamporttablemodel.h"

#include <constant.h>
#include <converters.h>
#include <customsnmpfunctions.h>
#include <ports/adslport.h>
#include <ports/shdslport.h>

// index.internalId()
// invalidParentIndex - основная информация о порте, верхний уровень
// > invalidParentIndex - дополнительная информация о порте, второй уровень number = index rowParent

DslamPortTableModel::DslamPortTableModel(Dslam::Ptr parentDevice, QObject *parent) :
    QAbstractItemModel(parent),
    mParentDevice(parentDevice)
{
}

int DslamPortTableModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return mList.size();
    } else if (parent.internalId() == invalidParentIndex) {
        return 7;
    } else {
        return 0;
    }
}

int DslamPortTableModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return 5;
    } else if (parent.internalId() == invalidParentIndex) {
        return 2;
    } else {
        return 0;
    }
}

QVariant DslamPortTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole) {
        return int(Qt::AlignCenter | Qt::AlignVCenter);
    } else if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (index.internalId() == invalidParentIndex) {
            return topLevelData(index);
        } else if (index.internalId() < invalidParentIndex) {
            return secondLevelData(index);
        }
    } else if ((role == Qt::BackgroundColorRole)
               && (index.internalId() == invalidParentIndex)) {
        if (mList[index.row()]->state() == DslPortState::Activating) {
            return QBrush(QColor(223, 255, 252));
        } else if (mList[index.row()]->state() == DslPortState::Up) {
            return QBrush(QColor(200, 255, 200));
        } else if (mList[index.row()]->state() == DslPortState::Down) {
            return QBrush(QColor(255, 145, 148));
        } else if (mList[index.row()]->state() == DslPortState::Defective) {
            return QBrush(QColor(255, 70, 74));
        }
    }

    return QVariant();
}

QVariant DslamPortTableModel::headerData(int section, Qt::Orientation orientation,
                                         int role) const
{
    if (orientation == Qt::Vertical)
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (section == 0)
            return DslamPortTableModelStrings::Pair;
        else if (section == 1)
            return DslamPortTableModelStrings::Port;
        else if (section == 2)
            return DslamPortTableModelStrings::State;
        else if (section == 3)
            return DslamPortTableModelStrings::Desc;
        else if (section == 4)
            return DslamPortTableModelStrings::Profile;
    } else if (role == Qt::TextAlignmentRole) {
        return int(Qt::AlignCenter | Qt::AlignVCenter);
    } else if (role == Qt::FontRole) {
        QFont font(qApp->font());
        font.setBold(true);
        return font;
    }

    return QVariant();
}

Qt::ItemFlags DslamPortTableModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex DslamPortTableModel::index(int row, int column,
                                       const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return createIndex(row, column, invalidParentIndex);
    } else if (parent.internalId() == invalidParentIndex) {
        return createIndex(row, column, parent.row());
    }

    return QModelIndex();
}

QModelIndex DslamPortTableModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    if (child.internalId() == invalidParentIndex) {
        return QModelIndex();
    } else if (child.internalId() < invalidParentIndex) {
        return createIndex((int)child.internalId(), 0, invalidParentIndex);
    }

    return QModelIndex();
}

BoardType::Enum DslamPortTableModel::boardType()
{
    return mBoardType;
}

void DslamPortTableModel::setBoardType(BoardType::Enum boardType)
{
    mBoardType = boardType;
}

void DslamPortTableModel::setFirstPair(int pair)
{
    mFirstPair = pair;
}

void DslamPortTableModel::setBoardIndex(int boardIndex)
{
    mBoardIndex = boardIndex;
}

bool DslamPortTableModel::load()
{
    beginResetModel();
    QScopedPointer<SnmpClient> snmpClient(new SnmpClient());

    snmpClient->setIp(mParentDevice->ip());

    if (!snmpClient->setupSession(SessionType::ReadSession)) {
        mError = SnmpErrorStrings::SetupSession;
        endResetModel();
        return false;
    }

    if (!snmpClient->openSession()) {
        mError = SnmpErrorStrings::OpenSession;
        endResetModel();
        return false;
    }

    snmpClient->createPdu(SNMP_MSG_GETNEXT);
    int portIndex = mParentDevice->snmpInterfaceIndex(mBoardType, mBoardIndex, 0) - 1;
    mList[0]->fillPrimaryLevelPdu(snmpClient.data(), portIndex);
    int size = mList.size();

    for (int i = 0; i < size; ++i) {
        if (snmpClient->sendRequest()) {
            if (!mList[i]->parsePrimaryLevelPdu(snmpClient.data())) {
                mError = QString::fromUtf8("При получении информации по портам произошли ошибки.");
                endResetModel();
                return false;
            } else {
                snmpClient->createPduFromResponse(SNMP_MSG_GETNEXT);
            }
        } else {
            mError = QString::fromUtf8("При получении информации по портам произошли ошибки.");
            endResetModel();
            return false;
        }
    }

    endResetModel();
    return true;
}

void DslamPortTableModel::createList()
{
    int count = mParentDevice->countPorts(mBoardType);
    mList.reserve(count);

    for (int i = 0; i < count; ++i) {
        XdslPort::Ptr port = mParentDevice->createPort(mBoardType, mBoardIndex,
                                                       i, this);

        mList.push_back(port);
        mList[i]->setPair(mFirstPair + i);
    }
}

bool DslamPortTableModel::updatePortInfo(QModelIndex portIndex)
{
    int currPort = currentPort(portIndex);

    if (currPort == -1) {
        mError = QString::fromUtf8("Не выбран порт для обновления информации");
        return false;
    }

    QScopedPointer<SnmpClient> snmpClient(new SnmpClient());

    snmpClient->setIp(mParentDevice->ip());

    if (!snmpClient->setupSession(SessionType::ReadSession)) {
        mError = SnmpErrorStrings::SetupSession;
        return false;
    }

    if (!snmpClient->openSession()) {
        mError = SnmpErrorStrings::OpenSession;
        return false;
    }

    snmpClient->createPdu(SNMP_MSG_GET);

    mList[currPort]->fillSecondaryLevelPdu(snmpClient.data());

    if (snmpClient->sendRequest()) {
        if (!mList[currPort]->parseSecondaryPrimaryLevelPdu(snmpClient.data())) {
            mError = SnmpErrorStrings::GetInfo;
            return false;
        } else {
            QModelIndex beginIndex = index(0, 0, portIndex);
            QModelIndex endIndex = index(0, 6, portIndex);
            emit dataChanged(beginIndex, endIndex);
            QModelIndex beginTopIndex = index(portIndex.row(), 1, QModelIndex());
            QModelIndex endTopIndex = index(portIndex.row(), 3, QModelIndex());
            emit dataChanged(beginTopIndex, endTopIndex);
        }
    } else {
        mError = SnmpErrorStrings::GetInfo;
        return false;
    }

    return true;
}

//PortState values
//1 - set up
//2 - set down
bool DslamPortTableModel::changePortState(int portIndex, int portState)
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

    long numInterface = mParentDevice->snmpInterfaceIndex(mBoardType, mBoardIndex,
                                                          portIndex);

    OidPair statusOid;
    if ((mParentDevice->deviceModel() == DeviceModel::MA5600)
            || (mParentDevice->deviceModel() == DeviceModel::MA5300)) {
        statusOid = createOidPair(Mib::ifAdminStatus, 10, numInterface);
    } else if (mParentDevice->deviceModel() == DeviceModel::MXA64) {
        statusOid = createOidPair(Mib::mxa64DslPortAdminStatus, 13, numInterface);
    } else if (mParentDevice->deviceModel() == DeviceModel::MXA32) {
        statusOid = createOidPair(Mib::mxa32DslPortAdminStatus, 13, numInterface);
    } else {
        mError = QString::fromUtf8("Ошибка : неизвестный тип дслама.");
        return false;
    }

    snmpClient->addOid(statusOid, QString::number(portState), 'i');

    return snmpClient->sendRequest();
}

bool DslamPortTableModel::changePortProfile(QModelIndex portIndex,
                                            QString profileName)
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

    int currPort = currentPort(portIndex);

    if (currPort == -1) {
        mError = "Необходимо выбрать порт для изменения профиля.";
        return false;
    }

    long indexPort = mParentDevice->snmpInterfaceIndex(mBoardType, mBoardIndex,
                                                        currPort);

    OidPair profileOid;
    char type;
    if ((mParentDevice->deviceModel() == DeviceModel::MA5600)
            || (mParentDevice->deviceModel() == DeviceModel::MA5300)) {
        profileOid = createOidPair(Mib::adslLineConfProfile, 13, indexPort);
        type = 's';
    } else if (mParentDevice->deviceModel() == DeviceModel::MXA64) {
        profileOid = createOidPair(Mib::mxa64DslPortActiveProfile, 13, indexPort);
        type = 'i';
    } else if (mParentDevice->deviceModel() == DeviceModel::MXA32) {
        profileOid =createOidPair(Mib::mxa32DslPortActiveProfile, 13, indexPort);
        type = 'i';
    } else {
        mError = QString::fromUtf8("Ошибка : неизвестный тип дслама!");
        return false;
    }

    snmpClient->addOid(profileOid, profileName, type);

    return snmpClient->sendRequest();
}

QString DslamPortTableModel::error() const
{
    return mError;
}

int DslamPortTableModel::currentPort(QModelIndex index)
{
    if (!index.isValid())
        return -1;

    if (index.internalId() == invalidParentIndex) {
        return index.row();
    } else {
        return index.parent().row();
    }
}

QVariant DslamPortTableModel::topLevelData(QModelIndex index) const
{
    if (index.column() == 0) {
        return mList.at(index.row())->pair();
    } else if (index.column() == 1) {
        return mList.at(index.row())->name();
    } else if (index.column() == 2) {
        return DslPortState::toString(mList.at(index.row())->state());
    } else if (index.column() == 3) {
        return mList.at(index.row())->description();
    } else if (index.column() == 4) {
        return mList.at(index.row())->profile();
    }

    return QVariant();
}

QVariant DslamPortTableModel::secondLevelData(QModelIndex index) const
{
    if (index.column() == 0) {
        if (index.row() == 0) {
            return DslamPortTableModelStrings::LineType;
        } else if (index.row() == 1) {
            return DslamPortTableModelStrings::RxRate;
        } else if (index.row() == 2) {
            return DslamPortTableModelStrings::TxRate;
        } else if (index.row() == 3) {
            return DslamPortTableModelStrings::RxAttenuation;
        } else if (index.row() == 4) {
            return DslamPortTableModelStrings::TxAttenuation;
        } else if (index.row() == 5) {
            return DslamPortTableModelStrings::LastChange;
        } else if (index.row() == 6) {
            return DslamPortTableModelStrings::Coding;
        }
    } else if (index.column() == 1) {
        AdslPort::Ptr portInfo = qobject_cast<AdslPort *>(mList.at(index.parent().row()));

        if (index.row() == 0) {
            return portInfo->lineType();
        } else if (index.row() == 1) {
            return portInfo->txRate();
        } else if (index.row() == 2) {
            return portInfo->rxRate();
        } else if (index.row() == 3) {
            return portInfo->txAttenuation();
        } else if (index.row() == 4) {
            return portInfo->rxAttenuation();
        } else if (index.row() == 5) {
            return portInfo->timeLastChange();
        } else if (index.row() == 6) {
            return portInfo->coding();
        }
    }

    return QVariant();
}
