#include "dslamporttablemodel.h"

#ifdef _MSC_VER
#include "../constant.h"
#include "../converters.h"
#include "../customsnmpfunctions.h"
#include "../ports/adslport.h"
#include "../ports/shdslport.h"
#else
#include "constant.h"
#include "converters.h"
#include "customsnmpfunctions.h"
#include "ports/adslport.h"
#include "ports/shdslport.h"
#endif


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

    long numInterface;

    int size = mList.size();
    //TODO: Refactoring on GET_NEXT request
    for (int i = 0; i < size; ++i) {
        snmpClient->createPdu(SNMP_MSG_GET);

        numInterface = mParentDevice->snmpInterfaceIndex(mBoardType, mBoardIndex, i);

        if ((mParentDevice->deviceModel() == DeviceModel::MA5600)
                || (mParentDevice->deviceModel() == DeviceModel::MA5300)) {
            if (mParentDevice->deviceModel() == DeviceModel::MA5300) {
                snmpClient->addOid(createOidPair(Mib::ifDescr, 10, numInterface));
            } else {
                snmpClient->addOid(createOidPair(Mib::ifName, 11, numInterface));
            }

            snmpClient->addOid(createOidPair(Mib::ifOperStatus, 10, numInterface));
            snmpClient->addOid(createOidPair(Mib::ifAlias, 11, numInterface));
            snmpClient->addOid(createOidPair(Mib::adslLineConfProfile, 13, numInterface));
        } else if (mParentDevice->deviceModel() == DeviceModel::MXA64) {
            snmpClient->addOid(createOidPair(Mib::mxa64DslPortOperStatus, 13,  numInterface));
            snmpClient->addOid(createOidPair(Mib::mxa64DslPortAdminStatus, 13, numInterface));
            snmpClient->addOid(createOidPair(Mib::mxa64DslPortName, 13, numInterface));
            snmpClient->addOid(createOidPair(Mib::mxa64DslPortActiveProfile, 13, numInterface));
        } else if (mParentDevice->deviceModel() == DeviceModel::MXA32) {
            snmpClient->addOid(createOidPair(Mib::mxa32DslPortOperStatus, 13, numInterface));
            snmpClient->addOid(createOidPair(Mib::mxa32DslPortAdminStatus, 13, numInterface));
            snmpClient->addOid(createOidPair(Mib::mxa32DslPortName, 13, numInterface));
            snmpClient->addOid(createOidPair(Mib::mxa32DslPortActiveProfile, 13, numInterface));
        } else {
            mError = QString::fromUtf8("Ошибка: Неизвестный тип дслама.");
            endResetModel();
            return false;
        }

        if (snmpClient->sendRequest()) {
            if ((mParentDevice->deviceModel() == DeviceModel::MA5600)
                    || (mParentDevice->deviceModel() == DeviceModel::MA5300)) {
                netsnmp_variable_list *vars = snmpClient->varList();
                if (isValidSnmpValue(vars)) {
                    mList[i]->setName(toString(vars->val.string, vars->val_len));
                }

                vars = vars->next_variable;
                if (isValidSnmpValue(vars)) {
                    mList[i]->setState(DslPortState::from(*vars->val.integer));
                }

                vars = vars->next_variable;
                if (isValidSnmpValue(vars)) {
                    QString desc = toString(vars->val.string, vars->val_len);
                    mList[i]->setDescription(desc);
                }

                vars = vars->next_variable;
                if (isValidSnmpValue(vars)) {
                    QString profile = toString(vars->val.string, vars->val_len);
                    mList[i]->setProfile(profileExtName(mParentDevice->deviceModel(), profile));
                }
            } else if ((mParentDevice->deviceModel() == DeviceModel::MXA64)
                       || (mParentDevice->deviceModel() == DeviceModel::MXA32)) {
                mList[i]->setName(QString("adsl %1").arg(numInterface));

                netsnmp_variable_list *vars = snmpClient->varList();

                if (isValidSnmpValue(vars)
                        && isValidSnmpValue(vars->next_variable)) {
                    DslPortState::Enum operStatus = DslPortState::from(*vars->val.integer);
                    DslPortState::Enum adminStatus = DslPortState::from(*vars->next_variable->val.integer);
                    DslPortState::Enum resultStatus;

                    if (adminStatus == DslPortState::Down)
                        resultStatus = DslPortState::Down;
                    else if ((adminStatus == DslPortState::Up)
                             && (operStatus == DslPortState::Up))
                        resultStatus = DslPortState::Up;
                    else if ((adminStatus == DslPortState::Up)
                             && (operStatus == DslPortState::Down))
                        resultStatus = DslPortState::Activating;
                    else
                        resultStatus = DslPortState::Other;

                    mList[i]->setState(resultStatus);
                }

                vars = vars->next_variable->next_variable;
                if (isValidSnmpValue(vars)) {
                    QString desc = toString(vars->val.string, vars->val_len);
                    mList[i]->setDescription(desc);
                }

                vars = vars->next_variable;
                if (isValidSnmpValue(vars)) {
                    QString profile = QString::number(*vars->val.integer);
                    mList[i]->setProfile(profileExtName(mParentDevice->deviceModel(), profile));
                }
            } else {
                mError = QString::fromUtf8("Ошибка: Неизвестный тип дслама.");
                endResetModel();
                return false;
            }
        } else {
            mError = SnmpErrorStrings::GetInfo;
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

    if ((mBoardType == BoardType::AnnexA)
            || (mBoardType == BoardType::AnnexB)) {

        for (int i = 0; i < count; ++i) {
            AdslPort::Ptr portInfo = new AdslPort(this);

            mList.push_back(portInfo);
            mList[i]->setPair(mFirstPair + i);
        }
    } else if (mBoardType == BoardType::Shdsl) {

        for (int i = 0; i < count; ++i) {
            ShdslPort::Ptr portInfo = new ShdslPort(this);

            mList.push_back(portInfo);
            mList[i]->setPair(mFirstPair + i);
        }
    }
}

bool DslamPortTableModel::updatePortInfo(QModelIndex portIndex)
{
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

    long numInterface;

    int currPort = currentPort(portIndex);

    if (currPort == -1) {
        mError = QString::fromUtf8("Не выбран порт для обновления информации");
        return false;
    }

    numInterface = mParentDevice->snmpInterfaceIndex(mBoardType, mBoardIndex,
                                                     currPort);

    if ((mParentDevice->deviceModel() == DeviceModel::MA5600)
            || (mParentDevice->deviceModel()  == DeviceModel::MA5300)) {
        snmpClient->addOid(createOidPair(Mib::ifOperStatus, 10, numInterface));
        snmpClient->addOid(createOidPair(Mib::adslAtucChanCurrTxRate, 13, numInterface));
        snmpClient->addOid(createOidPair(Mib::adslAturChanCurrTxRate, 13, numInterface));
        snmpClient->addOid(createOidPair(Mib::adslLineCoding, 13, numInterface));
        snmpClient->addOid(createOidPair(Mib::adslLineType, 13, numInterface));
        snmpClient->addOid(createOidPair(Mib::adslLineConfProfile, 13, numInterface));
        snmpClient->addOid(createOidPair(Mib::adslAtucCurrAtn, 13, numInterface));
        snmpClient->addOid(createOidPair(Mib::adslAturCurrAtn, 13, numInterface));
        snmpClient->addOid(createOidPair(Mib::ifLastChange, 10, numInterface));
    } else if (mParentDevice->deviceModel() == DeviceModel::MXA64) {
        snmpClient->addOid(createOidPair(Mib::mxa64DslPortOperStatus, 13, numInterface));
        snmpClient->addOid(createOidPair(Mib::mxa64DslPortAdminStatus, 13, numInterface));
        snmpClient->addOid(createOidPair(Mib::mxa64DslBandActualRateTx, 14, numInterface));
        snmpClient->addOid(createOidPair(Mib::mxa64DslBandActualRateRx, 14, numInterface));
        snmpClient->addOid(createOidPair(Mib::mxa64DslBandLineAttenuationTx, 14, numInterface));
        snmpClient->addOid(createOidPair(Mib::mxa64DslBandLineAttenuationRx, 14, numInterface));
        snmpClient->addOid(createOidPair(Mib::mxa64DslPortActiveProfile, 13, numInterface));
    } else if (mParentDevice->deviceModel() == DeviceModel::MXA32) {
        snmpClient->addOid(createOidPair(Mib::mxa32DslPortOperStatus, 13, numInterface));
        snmpClient->addOid(createOidPair(Mib::mxa32DslPortAdminStatus, 13, numInterface));
        snmpClient->addOid(createOidPair(Mib::mxa32DslBandActualRateTx, 14, numInterface));
        snmpClient->addOid(createOidPair(Mib::mxa32DslBandActualRateRx, 14, numInterface));
        snmpClient->addOid(createOidPair(Mib::mxa32DslBandLineAttenuationTx, 14, numInterface));
        snmpClient->addOid(createOidPair(Mib::mxa32DslBandLineAttenuationRx, 14, numInterface));
        snmpClient->addOid(createOidPair(Mib::mxa32DslPortActiveProfile, 13, numInterface));
    } else {
        mError = QString::fromUtf8("Ошибка: Неизвестный тип дслама!");
        return false;
    }

    bool result = true;

    if (snmpClient->sendRequest()) {
        if ((mParentDevice->deviceModel() == DeviceModel::MA5600)
                || (mParentDevice->deviceModel() == DeviceModel::MA5300))
            result = updatePortMA(portIndex, snmpClient);
        else if ((mParentDevice->deviceModel() == DeviceModel::MXA64)
                 || (mParentDevice->deviceModel() == DeviceModel::MXA32))
            result = updatePortMXA(portIndex, snmpClient);
        else {
            mError = QString::fromUtf8("Ошибка: Неизвестный тип дслама!");
            return false;
        }

        if (!result)
        {
            mError = SnmpErrorStrings::GetInfo;
            return false;
        }

        QModelIndex beginIndex = index(0, 0, portIndex);
        QModelIndex endIndex = index(0, 6, portIndex);
        emit dataChanged(beginIndex, endIndex);
        QModelIndex beginTopIndex = index(portIndex.row(), 1, QModelIndex());
        QModelIndex endTopIndex = index(portIndex.row(), 3, QModelIndex());
        emit dataChanged(beginTopIndex, endTopIndex);
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

bool DslamPortTableModel::updatePortMA(QModelIndex portIndex,
                                       QScopedPointer<SnmpClient> &snmp)
{
    AdslPort::Ptr portInfo = static_cast<AdslPort *>(mList[portIndex.row()]);
    netsnmp_variable_list *vars = snmp->varList();

    portInfo->setState(DslPortState::from(*vars->val.integer));

    if (portInfo->state() == DslPortState::Up) {
        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        portInfo->setTxRate(*vars->val.integer / 1000);

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        portInfo->setRxRate(*vars->val.integer / 1000);
    } else {
        portInfo->setTxRate(0);
        portInfo->setRxRate(0);
        vars = vars->next_variable->next_variable;
        if (!isValidSnmpValue(vars))
            return false;
        //пропускаем vars
    }

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    portInfo->setCoding(codingString(*vars->val.integer));

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    portInfo->setLineType(typeLineString(*vars->val.integer));

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    QString profile = toString(vars->val.string, vars->val_len);
    portInfo->setProfile(profileExtName(mParentDevice->deviceModel(), profile));

    if (portInfo->state() == DslPortState::Up) {
        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        portInfo->setTxAttenuation(QString::number(*vars->val.integer / 10.0));

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        portInfo->setRxAttenuation(QString::number(*vars->val.integer / 10.0));
    } else {
        portInfo->setTxAttenuation(QString());
        portInfo->setRxAttenuation(QString());
        vars = vars->next_variable->next_variable;
        if (!isValidSnmpValue(vars))
            return false;
    }

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    QDateTime date = QDateTime::currentDateTime();
    date = date.addSecs(-*vars->val.integer / 100);

    portInfo->setTimeLastChange(date.toString("dd.MM.yyyy hh:mm"));

    return true;
}

bool DslamPortTableModel::updatePortMXA(QModelIndex portIndex,
                                        QScopedPointer<SnmpClient> &snmp)
{
    AdslPort::Ptr portInfo = static_cast<AdslPort *>(mList[portIndex.row()]);
    netsnmp_variable_list *vars = snmp->varList();

    if (isValidSnmpValue(vars)
            && isValidSnmpValue(vars->next_variable)) {
        DslPortState::Enum operStatus = DslPortState::from(*vars->val.integer);
        DslPortState::Enum adminStatus = DslPortState::from(*vars->next_variable->val.integer);
        DslPortState::Enum resultStatus;

        if (adminStatus == DslPortState::Down)
            resultStatus = DslPortState::Down;
        else if ((adminStatus == DslPortState::Up)
                 && (operStatus == DslPortState::Up))
            resultStatus = DslPortState::Up;
        else if ((adminStatus == DslPortState::Up)
                 && (operStatus == DslPortState::Down))
            resultStatus = DslPortState::Activating;
        else
            resultStatus = DslPortState::Other;

        portInfo->setState(resultStatus);
    }

    if (portInfo->state() != DslPortState::Up) {
        portInfo->setTxRate(0);
        portInfo->setRxRate(0);
        portInfo->setTxAttenuation("");
        portInfo->setRxAttenuation("");
        vars = vars->next_variable->next_variable->next_variable->next_variable->next_variable;
    }
    else {
        vars = vars->next_variable->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        portInfo->setTxRate(*vars->val.integer / 1000);

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        portInfo->setRxRate(*vars->val.integer / 1000);

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        portInfo->setTxAttenuation(QString::number(*vars->val.integer));

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        portInfo->setRxAttenuation(QString::number(*vars->val.integer));
    }

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    QString profile = QString::number(*vars->val.integer);
    portInfo->setProfile(profileExtName(mParentDevice->deviceModel(), profile));

    return true;
}
