#include "dslamporttablemodel.h"

#include <basicdialogs.h>
#include <constant.h>
#include <converters.h>
#include <customsnmpfunctions.h>
#include <ports/adslport.h>
#include <ports/shdslport.h>

// index.internalId()
// invalidParentIndex - основная информация о порте, верхний уровень
// < invalidParentIndex - дополнительная информация о порте, второй уровень number = index rowParent

DslamPortTableModel::DslamPortTableModel(Dslam::Ptr parentDevice,
                                         ImprovedMessageWidget *messageWidget, QObject *parent) :
    QAbstractItemModel(parent),
    mParentDevice(parentDevice),
    mFutureWatcher(new QFutureWatcher<void>()),
    mMessageWidget(messageWidget)
{
    connect(mFutureWatcher, &QFutureWatcher<void>::finished,
            this, &DslamPortTableModel::finishAsyncUpdate);
}

DslamPortTableModel::~DslamPortTableModel()
{
    if (mFutureWatcher) {
        if (mFutureWatcher->isRunning()) {
            mFutureWatcher->cancel();
            mFutureWatcher->waitForFinished();
        }

        delete mFutureWatcher;
    }
}

int DslamPortTableModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return mList.size();
    } else if (parent.internalId() == invalidParentIndex) {
        if ((mBoardType == BoardType::AnnexA)
                || (mBoardType == BoardType::AnnexB)) {
            return 7;
        } else if (mBoardType == BoardType::Shdsl) {
            return 6;
        } else {
            return 0;
        }
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

bool DslamPortTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    if ((!index.isValid()) || (role != Qt::EditRole))
        return false;

    if ((index.column() != DescColumn)
            || (index.internalId() != invalidParentIndex))
        return false;

    if (value.toString() == mList[index.row()]->description())
        return true;

    long portIndex = mParentDevice->snmpPortIndex(mBoardType, mBoardIndex, index.row());
    bool result = mParentDevice->setPortDescription(portIndex, value.toString());

    if (result) {
        mList[index.row()]->setDescription(value.toString());
        emit dataChanged(index, index);
    } else {
        mMessageWidget->setMessageType(ImprovedMessageWidget::Error);
        mMessageWidget->setText(mParentDevice->error());
        mMessageWidget->animatedShow();
    }

    return true;
}

QVariant DslamPortTableModel::headerData(int section, Qt::Orientation orientation,
                                         int role) const
{
    if (orientation == Qt::Vertical)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (section) {
        case PairColumn:
            return DslamPortTableModelStrings::Pair;
        case PortColumn:
            return DslamPortTableModelStrings::Port;
        case StateColumn:
            return DslamPortTableModelStrings::State;
        case DescColumn:
            return DslamPortTableModelStrings::Desc;
        case ProfileColumn:
            return DslamPortTableModelStrings::Profile;
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

Qt::ItemFlags DslamPortTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if ((index.column() == DescColumn)
            && (index.internalId() == invalidParentIndex))
        flags |= Qt::ItemIsEditable;

    return flags;
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

bool DslamPortTableModel::updatePortExtended(QModelIndex portIndex)
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
        if (!mList[currPort]->parseSecondaryLevelPdu(snmpClient.data())) {
            mError = SnmpErrorStrings::Parse;
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
        mError = snmpClient->error();
        return false;
    }

    return true;
}

bool DslamPortTableModel::updatePortBasic(QModelIndex portIndex)
{
    int currPort = currentPort(portIndex);

    if (currPort == -1) {
        mError = QString::fromUtf8("Не выбран порт для обновления информации");
        return false;
    }

    mUpdateErrors.clear();

    bool result = updatePortBasic(mList[currPort]);

    if (!result)
        mError = mUpdateErrors;

    return result;
}

bool DslamPortTableModel::updatePortBasic(const XdslPort::Ptr &port)
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

    port->fillPrimaryLevelPdu(snmpClient.data());

    if (snmpClient->sendRequest()) {
        if (!port->parsePrimaryLevelPdu(snmpClient.data())) {
            appendUpdateError(QString::fromUtf8("Информацию по порту %1 получить не удалось. %2")
                              .arg(port->index())
                              .arg(SnmpErrorStrings::Parse));
            return false;
        } else {
            QModelIndex beginTopIndex = index(port->index(), 0, QModelIndex());
            QModelIndex endTopIndex = index(port->index(), 4, QModelIndex());
            emit dataChanged(beginTopIndex, endTopIndex);
        }
    } else {
        appendUpdateError(QString::fromUtf8("Информацию по порту %1 получить не удалось. %2")
                          .arg(port->index())
                          .arg(snmpClient->error()));
        return false;
    }

    return true;
}

void DslamPortTableModel::update()
{
    mUpdateErrors.clear();

    if (mFutureWatcher->isRunning()) {
        mError = QString::fromUtf8("Обновление информации по портам уже выполняется.");
        emit updateFinished(true);
        return;
    }

    QFuture<void> future = QtConcurrent::map(mList, DslamUpdateWrapperObject(this));
    mFutureWatcher->setFuture(future);
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

    long numInterface = mParentDevice->snmpPortIndex(mBoardType, mBoardIndex,
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

    long indexPort = mParentDevice->snmpPortIndex(mBoardType, mBoardIndex,
                                                        currPort);

    OidPair profileOid;
    char type;
    if ((mBoardType == BoardType::AnnexA)
            || (mBoardType == BoardType::AnnexB)) {
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
    } else if (mBoardType ==BoardType::Shdsl) {
        if ((mParentDevice->deviceModel() == DeviceModel::MA5600)
                || (mParentDevice->deviceModel() == DeviceModel::MA5300)) {
            profileOid = createOidPair(Mib::hdsl2ShdslSpanConfProfile, 12, indexPort);
            type = 's';
        } else {
            mError = QString::fromUtf8("Ошибка : неизвестный тип дслама!");
            return false;
        }
    } else {
        mError = QString::fromUtf8("Ошибка : неподдерживаемый тип доски!");
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
    switch(index.column()) {
    case PairColumn:
        return mList.at(index.row())->pair();
    case PortColumn:
        return mList.at(index.row())->name();
    case StateColumn:
        return DslPortState::toString(mList.at(index.row())->state());
    case DescColumn:
        return mList.at(index.row())->description();
    case ProfileColumn:
        return mList.at(index.row())->profile();
    default:
        return QVariant();
    }
}

QVariant DslamPortTableModel::secondLevelData(QModelIndex index) const
{
    if ((mBoardType == BoardType::AnnexA)
            || (mBoardType == BoardType::AnnexB)) {
        return secondLevelDataAdsl(index);
    } else if (mBoardType == BoardType::Shdsl) {
        return secondLevelDataShdsl(index);
    } else {
        return QVariant();
    }
}

QVariant DslamPortTableModel::secondLevelDataAdsl(QModelIndex index) const
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
            return QString::fromUtf8("%1/%2/%3")
                    .arg(portInfo->txAttainableRate())
                    .arg(portInfo->txCurrRate())
                    .arg(portInfo->txPrevRate());
        } else if (index.row() == 2) {
            return QString::fromUtf8("%1/%2/%3")
                    .arg(portInfo->rxAttainableRate())
                    .arg(portInfo->rxCurrRate())
                    .arg(portInfo->rxPrevRate());
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

QVariant DslamPortTableModel::secondLevelDataShdsl(QModelIndex index) const
{
    if (index.column() == 0) {
        if (index.row() == 0) {
            return DslamPortTableModelStrings::TransmissionMode;
        } else if (index.row() == 1) {
            return DslamPortTableModelStrings::ActualRate;
        } else if (index.row() == 2) {
            return DslamPortTableModelStrings::MaxRate;
        } else if (index.row() == 3) {
            return DslamPortTableModelStrings::Attenuation;
        } else if (index.row() == 4) {
            return DslamPortTableModelStrings::SnrMargin;
        } else if (index.row() == 5) {
            return DslamPortTableModelStrings::LastChange;
        }
    } else if (index.column() == 1) {
        ShdslPort::Ptr portInfo = qobject_cast<ShdslPort *>(mList.at(index.parent().row()));

        if (index.row() == 0) {
            return portInfo->transmissonMode();
        } else if (index.row() == 1) {
            return portInfo->actualRate();
        } else if (index.row() == 2) {
            return portInfo->maxRate();
        } else if (index.row() == 3) {
            return portInfo->rxAttenuation();
        } else if (index.row() == 4) {
            return portInfo->snrMargin();
        } else if (index.row() == 5) {
            return portInfo->timeLastChange();
        }
    }

    return QVariant();
}

void DslamPortTableModel::finishAsyncUpdate()
{
    if (!mUpdateErrors.isEmpty())
        mError = mUpdateErrors;

    emit updateFinished(!mUpdateErrors.isEmpty());
}
