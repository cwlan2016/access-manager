#include "dslamportlistmodel.h"
#include <QtCore/QDateTime>
#include <QtCore/QStringBuilder>
#include <QtGui/QBrush>
#include <QtGui/QFont>
#include <QtWidgets/QApplication>
#ifdef _MSC_VER
#include "../constant.h"
#include "../converters.h"
#include "../customsnmpfunctions.h"
#include "../Info/adslportinfo.h"
#include "../Info/shdslportinfo.h"
#else
#include "constant.h"
#include "converters.h"
#include "customsnmpfunctions.h"
#include "Info/adslportinfo.h"
#include "Info/shdslportinfo.h"
#endif


// index.internalId()
// invalidParentIndex - основная информация о порте, верхний уровень
// > invalidParentIndex - дополнительная информация о порте, второй уровень number = index rowParent

DslamPortListModel::DslamPortListModel(DeviceModel::Enum deviceModel, QString ip, QObject *parent) :
    QAbstractItemModel(parent), mIp(ip), mDeviceModel(deviceModel)
{
}

int DslamPortListModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return mPortList.size();
    else if (parent.internalId() == invalidParentIndex)
        return 7;
    else
        return 0;
}

int DslamPortListModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 5;
    else if (parent.internalId() == invalidParentIndex)
        return 2;
    else
        return 0;
}

QVariant DslamPortListModel::data(const QModelIndex &index, int role) const
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
        } else
            return QVariant();
    } else if ((role == Qt::BackgroundColorRole) && (index.internalId() == invalidParentIndex)) {
        if (mPortList[index.row()]->state() == "Activating") {
            return QBrush(QColor(223, 255, 252));
        } else if (mPortList[index.row()]->state() == "Up") {
            return QBrush(QColor(200, 255, 200));
        } else if (mPortList[index.row()]->state() == "Down") {
            return QBrush(QColor(255, 145, 148));
        } else if (mPortList[index.row()]->state() == "Defective") {
            return QBrush(QColor(255, 70, 74));
        } else
            return QVariant();
    } else
        return QVariant();
}

QVariant DslamPortListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical)
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (section == 0)
            return DslamPortListModelColumn::Pair;
        else if (section == 1)
            return DslamPortListModelColumn::Port;
        else if (section == 2)
            return DslamPortListModelColumn::State;
        else if (section == 3)
            return DslamPortListModelColumn::Desc;
        else if (section == 4)
            return DslamPortListModelColumn::Profile;
        else
            return QVariant();
    } else if (role == Qt::TextAlignmentRole) {
        return int(Qt::AlignCenter | Qt::AlignVCenter);
    } else if (role == Qt::FontRole) {
        QFont font(qApp->font());
        font.setBold(true);
        return font;
    } else
        return QVariant();
}

Qt::ItemFlags DslamPortListModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex DslamPortListModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return createIndex(row, column, invalidParentIndex);
    } else if (parent.internalId() == invalidParentIndex) {
        return createIndex(row, column, parent.row());
    } else
        return QModelIndex();
}

QModelIndex DslamPortListModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    if (child.internalId() == invalidParentIndex) {
        return QModelIndex();
    } else if (child.internalId() < invalidParentIndex) {
        return createIndex((int)child.internalId(), 0, invalidParentIndex);
    } else
        return QModelIndex();
}

QString DslamPortListModel::error() const
{
    return mError;
}

BoardType::Enum DslamPortListModel::boardType()
{
    return mBoardType;
}

void DslamPortListModel::setBoardType(BoardType::Enum boardType)
{
    mBoardType = boardType;
}

void DslamPortListModel::setFirstPair(int pair)
{
    mFirstPair = pair;
}

void DslamPortListModel::setBoardNumber(int boardNum)
{
    mBoardNumber = boardNum;
}

bool DslamPortListModel::load()
{
    beginResetModel();
    QScopedPointer<SnmpClient> snmp(new SnmpClient());

    snmp->setIp(mIp);

    if (!snmp->setupSession(SessionType::ReadSession)) {
        mError = SnmpErrors::SetupSession;
        endResetModel();
        return false;
    }

    if (!snmp->openSession()) {
        mError = SnmpErrors::OpenSession;
        endResetModel();
        return false;
    }

    long numInterface;

    int size = mPortList.size();
    for (int i = 0; i < size; ++i) {
        snmp->createPdu(SNMP_MSG_GET);

        numInterface = snmpInterfaceNumber(mDeviceModel, mBoardNumber, i);

        if ((mDeviceModel == DeviceModel::MA5600)
                || (mDeviceModel == DeviceModel::MA5300)) {
            if (mDeviceModel == DeviceModel::MA5300) {
                snmp->addOid(createOid(Mib::ifDescr, 10, numInterface), 11);
            } else {
                snmp->addOid(createOid(Mib::ifName, 11, numInterface), 12);
            }

            snmp->addOid(createOid(Mib::ifOperStatus, 10, numInterface), 11);
            snmp->addOid(createOid(Mib::ifAlias, 11, numInterface), 12);
            snmp->addOid(createOid(Mib::adslLineConfProfile, 13, numInterface), 14);
        } else if (mDeviceModel == DeviceModel::MXA64) {
            snmp->addOid(createOid(Mib::mxa64DslPortOperStatus, 13,  numInterface), 14);
            snmp->addOid(createOid(Mib::mxa64DslPortAdminStatus, 13, numInterface), 14);
            snmp->addOid(createOid(Mib::mxa64DslPortName, 13, numInterface), 14);
            snmp->addOid(createOid(Mib::mxa64DslPortActiveProfile, 13, numInterface), 14);
        } else if (mDeviceModel == DeviceModel::MXA32) {
            snmp->addOid(createOid(Mib::mxa32DslPortOperStatus, 13, numInterface), 14);
            snmp->addOid(createOid(Mib::mxa32DslPortAdminStatus, 13, numInterface), 14);
            snmp->addOid(createOid(Mib::mxa32DslPortName, 13, numInterface), 14);
            snmp->addOid(createOid(Mib::mxa32DslPortActiveProfile, 13, numInterface), 14);
        } else {
            mError = QString::fromUtf8("Ошибка: Неизвестный тип дслама.");
            endResetModel();
            return false;
        }

        if (snmp->sendRequest()) {
            if ((mDeviceModel == DeviceModel::MA5600)
                    || (mDeviceModel == DeviceModel::MA5300)) {
                netsnmp_variable_list *vars = snmp->varList();
                if (isValidSnmpValue(vars)) {
                    mPortList[i]->setName(toQString(vars->val.string, vars->val_len));
                }

                vars = vars->next_variable;
                if (isValidSnmpValue(vars)) {
                    mPortList[i]->setState(dslamStatePortString(*vars->val.integer));
                }

                vars = vars->next_variable;
                if (isValidSnmpValue(vars)) {
                    mPortList[i]->setDesc(toQString(vars->val.string, vars->val_len));
                }

                vars = vars->next_variable;
                if (isValidSnmpValue(vars)) {
                    mPortList[i]->setProfile(profileExtName(mDeviceModel, toQString(vars->val.string, vars->val_len)));
                }
            } else if ((mDeviceModel == DeviceModel::MXA64)
                       || (mDeviceModel == DeviceModel::MXA32)) {
                mPortList[i]->setName("adsl " % QString::number(numInterface));

                netsnmp_variable_list *vars = snmp->varList();

                if (isValidSnmpValue(vars)
                        && isValidSnmpValue(vars->next_variable)) {
                    QString operStatus = dslamStatePortString(*vars->val.integer);
                    QString adminStatus = dslamStatePortString(*vars->next_variable->val.integer);
                    QString resultStatus;

                    if (adminStatus == "Down")
                        resultStatus = "Down";
                    else if ((adminStatus == "Up") && (operStatus == "Up"))
                        resultStatus = "Up";
                    else if ((adminStatus == "Up") && (operStatus == "Down"))
                        resultStatus = "Activating";
                    else
                        resultStatus = "Other";

                    mPortList[i]->setState(resultStatus);
                }

                vars = vars->next_variable->next_variable;
                if (isValidSnmpValue(vars)) {
                    mPortList[i]->setDesc(toQString(vars->val.string, vars->val_len));
                }

                vars = vars->next_variable;
                if (isValidSnmpValue(vars)) {
                    mPortList[i]->setProfile(profileExtName(mDeviceModel, QString::number(*vars->val.integer)));
                }
            } else {
                mError = QString::fromUtf8("Ошибка: Неизвестный тип дслама.");
                endResetModel();
                return false;
            }
        } else {
            mError = SnmpErrors::GetInfo;
            endResetModel();
            return false;
        }
    }

    endResetModel();
    return true;
}

void DslamPortListModel::createList()
{
    int count = countPorts(mDeviceModel, mBoardType);

    if ((mBoardType == BoardType::AnnexA)
            || (mBoardType == BoardType::AnnexB)) {
        mPortList.clear();

        for (int i = 0; i < count; ++i) {
            AdslPortInfo::Ptr portInfo = std::make_shared<AdslPortInfo>();

            mPortList.push_back(std::move(portInfo));
            mPortList[i]->setPair(mFirstPair + i);
        }
    } else if (mBoardType == BoardType::Shdsl) {
        mPortList.clear();

        for (int i = 0; i < count; ++i) {
            ShdslPortInfo::Ptr portInfo = std::make_shared<ShdslPortInfo>();

            mPortList.push_back(portInfo);
            mPortList[i]->setPair(mFirstPair + i);
        }
    }
}

bool DslamPortListModel::updatePortInfo(QModelIndex portIndex)
{
    QScopedPointer<SnmpClient> snmp(new SnmpClient());

    snmp->setIp(mIp);

    if (!snmp->setupSession(SessionType::ReadSession)) {
        mError = SnmpErrors::SetupSession;
        return false;
    }

    if (!snmp->openSession()) {
        mError = SnmpErrors::OpenSession;
        return false;
    }

    snmp->createPdu(SNMP_MSG_GET);

    long numInterface;

    int currPort = currentPort(portIndex);

    if (currPort == -1) {
        mError = QString::fromUtf8("Не выбран порт для обновления информации");
        return false;
    }

    numInterface = snmpInterfaceNumber(mDeviceModel, mBoardNumber, currPort);

    if ((mDeviceModel == DeviceModel::MA5600)
            || (mDeviceModel  == DeviceModel::MA5300)) {
        snmp->addOid(createOid(Mib::ifOperStatus, 10, numInterface), 11);
        snmp->addOid(createOid(Mib::adslAtucChanCurrTxRate, 13, numInterface), 14);
        snmp->addOid(createOid(Mib::adslAturChanCurrTxRate, 13, numInterface), 14);
        snmp->addOid(createOid(Mib::adslLineCoding, 13, numInterface), 14);
        snmp->addOid(createOid(Mib::adslLineType, 13, numInterface), 14);
        snmp->addOid(createOid(Mib::adslLineConfProfile, 13, numInterface), 14);
        snmp->addOid(createOid(Mib::adslAtucCurrAtn, 13, numInterface), 14);
        snmp->addOid(createOid(Mib::adslAturCurrAtn, 13, numInterface), 14);
        snmp->addOid(createOid(Mib::ifLastChange, 10, numInterface), 11);
    } else if (mDeviceModel == DeviceModel::MXA64) {
        snmp->addOid(createOid(Mib::mxa64DslPortOperStatus, 13, numInterface), 14);
        snmp->addOid(createOid(Mib::mxa64DslPortAdminStatus, 13, numInterface), 14);
        snmp->addOid(createOid(Mib::mxa64DslBandActualRateTx, 14, numInterface), 15);
        snmp->addOid(createOid(Mib::mxa64DslBandActualRateRx, 14, numInterface), 15);
        snmp->addOid(createOid(Mib::mxa64DslBandLineAttenuationTx, 14, numInterface), 15);
        snmp->addOid(createOid(Mib::mxa64DslBandLineAttenuationRx, 14, numInterface), 15);
        snmp->addOid(createOid(Mib::mxa64DslPortActiveProfile, 13, numInterface), 14);
    } else if (mDeviceModel == DeviceModel::MXA32) {
        snmp->addOid(createOid(Mib::mxa32DslPortOperStatus, 13, numInterface), 14);
        snmp->addOid(createOid(Mib::mxa32DslPortAdminStatus, 13, numInterface), 14);
        snmp->addOid(createOid(Mib::mxa32DslBandActualRateTx, 14, numInterface), 15);
        snmp->addOid(createOid(Mib::mxa32DslBandActualRateRx, 14, numInterface), 15);
        snmp->addOid(createOid(Mib::mxa32DslBandLineAttenuationTx, 14, numInterface), 15);
        snmp->addOid(createOid(Mib::mxa32DslBandLineAttenuationRx, 14, numInterface), 15);
        snmp->addOid(createOid(Mib::mxa32DslPortActiveProfile, 13, numInterface), 14);
    } else {
        mError =  QString::fromUtf8("Ошибка: Неизвестный тип дслама!");
        return false;
    }

    bool result = true;

    if (snmp->sendRequest()) {
        if ((mDeviceModel == DeviceModel::MA5600)
                || (mDeviceModel == DeviceModel::MA5300))
            result = updatePortMA(portIndex, snmp);
        else if ((mDeviceModel == DeviceModel::MXA64)
                 || (mDeviceModel == DeviceModel::MXA32))
            result = updatePortMXA(portIndex, snmp);
        else {
            mError = QString::fromUtf8("Ошибка: Неизвестный тип дслама!");
            return false;
        }

        if (!result)
        {
            mError = SnmpErrors::GetInfo;
            return false;
        }

        QModelIndex beginIndex = index(0, 0, portIndex);
        QModelIndex endIndex = index(0, 6, portIndex);
        emit dataChanged(beginIndex, endIndex);
        QModelIndex beginTopIndex = index(portIndex.row(), 1, QModelIndex());
        QModelIndex endTopIndex = index(portIndex.row(), 3, QModelIndex());
        emit dataChanged(beginTopIndex, endTopIndex);
    } else {
        mError = SnmpErrors::GetInfo;
        return false;
    }

    return true;
}

//PortState values
//1 - set up
//2 - set down
bool DslamPortListModel::changePortState(int portIndex, QString portState)
{
    QScopedPointer<SnmpClient> snmp(new SnmpClient());

    snmp->setIp(mIp);

    if (!snmp->setupSession(SessionType::WriteSession)) {
        mError = SnmpErrors::SetupSession;
        return false;
    }

    if (!snmp->openSession()) {
        mError = SnmpErrors::OpenSession;
        return false;
    }

    snmp->createPdu(SNMP_MSG_SET);

    long numInterface = snmpInterfaceNumber(mDeviceModel, mBoardNumber, portIndex);

    if ((mDeviceModel == DeviceModel::MA5600) || (mDeviceModel == DeviceModel::MA5300))
        snmp->addOid(createOid(Mib::ifAdminStatus, 10, numInterface), 11, portState, 'i');
    else if (mDeviceModel == DeviceModel::MXA64)
        snmp->addOid(createOid(Mib::mxa64DslPortAdminStatus, 13, numInterface), 14, portState, 'i');
    else if (mDeviceModel == DeviceModel::MXA32)
        snmp->addOid(createOid(Mib::mxa32DslPortAdminStatus, 13, numInterface), 14, portState, 'i');
    else {
        mError = QString::fromUtf8("Ошибка : неизвестный тип дслама.");
        return false;
    }

    return snmp->sendRequest();
}

bool DslamPortListModel::changePortProfile(QModelIndex portIndex, QString profileName)
{
    QScopedPointer<SnmpClient> snmp(new SnmpClient());

    snmp->setIp(mIp);

    if (!snmp->setupSession(SessionType::WriteSession)) {
        mError = SnmpErrors::SetupSession;
        return false;
    }

    if (!snmp->openSession()) {
        mError = SnmpErrors::OpenSession;
        return false;
    }

    snmp->createPdu(SNMP_MSG_SET);

    int currPort = currentPort(portIndex);

    if (currPort == -1) {
        mError = "Необходимо выбрать порт для изменения профиля.";
        return false;
    }

    long numInterface = snmpInterfaceNumber(mDeviceModel, mBoardNumber, currPort);

    if ((mDeviceModel == DeviceModel::MA5600) || (mDeviceModel == DeviceModel::MA5300))
        snmp->addOid(createOid(Mib::adslLineConfProfile, 13, numInterface), 14, profileName, 's');
    else if (mDeviceModel == DeviceModel::MXA64)
        snmp->addOid(createOid(Mib::mxa64DslPortActiveProfile, 13, numInterface), 14, profileName, 'i');
    else if (mDeviceModel == DeviceModel::MXA32)
        snmp->addOid(createOid(Mib::mxa32DslPortActiveProfile, 13, numInterface), 14, profileName, 'i');
    else {
        mError = QString::fromUtf8("Ошибка : неизвестный тип дслама!");
        return false;
    }

    return snmp->sendRequest();
}

QVariant DslamPortListModel::topLevelData(QModelIndex index) const
{
    if (index.column() == 0)
        return mPortList[index.row()]->pair();
    else if (index.column() == 1)
        return mPortList[index.row()]->name();
    else if (index.column() == 2)
        return mPortList[index.row()]->state();
    else if (index.column() == 3)
        return mPortList[index.row()]->desc();
    else if (index.column() == 4)
        return mPortList[index.row()]->profile();
    else
        return QVariant();
}

QVariant DslamPortListModel::secondLevelData(QModelIndex index) const
{
    if (index.column() == 0) {
        if (index.row() == 0)
            return DslamPortListModelColumn::LineType;
        else if (index.row() == 1)
            return DslamPortListModelColumn::RxRate;
        else if (index.row() == 2)
            return DslamPortListModelColumn::TxRate;
        else if (index.row() == 3)
            return DslamPortListModelColumn::RxAttenuation;
        else if (index.row() == 4)
            return DslamPortListModelColumn::TxAttenuation;
        else if (index.row() == 5)
            return DslamPortListModelColumn::LastChange;
        else if (index.row() == 6)
            return DslamPortListModelColumn::Coding;
        else
            return QVariant();
    } else if (index.column() == 1) {
        if (index.row() == 0)
            return std::static_pointer_cast<AdslPortInfo>(mPortList[index.parent().row()])->lineType();
        else if (index.row() == 1)
            return std::static_pointer_cast<AdslPortInfo>(mPortList[index.parent().row()])->txRate();
        else if (index.row() == 2)
            return std::static_pointer_cast<AdslPortInfo>(mPortList[index.parent().row()])->rxRate();
        else if (index.row() == 3)
            return mPortList[index.parent().row()]->txAttenuation();
        else if (index.row() == 4)
            return mPortList[index.parent().row()]->rxAttenuation();
        else if (index.row() == 5)
            return mPortList[index.parent().row()]->timeLastChange();
        else if (index.row() == 6)
            return std::static_pointer_cast<AdslPortInfo>(mPortList[index.parent().row()])->coding();
        else
            return QVariant();
    } else
        return QVariant();
}

bool DslamPortListModel::updatePortMA(QModelIndex portIndex, QScopedPointer<SnmpClient> &snmp)
{
    netsnmp_variable_list *vars = snmp->varList();

    mPortList[portIndex.row()]->setState(dslamStatePortString(*vars->val.integer));

    if (mPortList[portIndex.row()]->state() == "Up") {
        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        std::static_pointer_cast<AdslPortInfo>(mPortList[portIndex.row()])->setTxRate(*vars->val.integer / 1000);

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        std::static_pointer_cast<AdslPortInfo>(mPortList[portIndex.row()])->setRxRate(*vars->val.integer / 1000);
    } else {
        std::static_pointer_cast<AdslPortInfo>(mPortList[portIndex.row()])->setTxRate(0);
        std::static_pointer_cast<AdslPortInfo>(mPortList[portIndex.row()])->setRxRate(0);
        vars = vars->next_variable->next_variable;
        if (!isValidSnmpValue(vars))
            return false;
        //пропускаем vars
    }

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    std::static_pointer_cast<AdslPortInfo>(mPortList[portIndex.row()])->setCoding(codingString(*vars->val.integer));

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    std::static_pointer_cast<AdslPortInfo>(mPortList[portIndex.row()])->setLineType(typeLineString(*vars->val.integer));

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    mPortList[portIndex.row()]->setProfile(profileExtName(mDeviceModel, toQString(vars->val.string, vars->val_len)));

    if (mPortList[portIndex.row()]->state() == "Up") {
        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        mPortList[portIndex.row()]->setTxAttenuation(QString::number(*vars->val.integer / 10.0));

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        mPortList[portIndex.row()]->setRxAttenuation(QString::number(*vars->val.integer / 10.0));
    } else {
        mPortList[portIndex.row()]->setTxAttenuation("");
        mPortList[portIndex.row()]->setRxAttenuation("");
        vars = vars->next_variable->next_variable;
        if (!isValidSnmpValue(vars))
            return false;
    }

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    QDateTime date = QDateTime::currentDateTime();
    date = date.addSecs(-*vars->val.integer / 100);

    mPortList[portIndex.row()]->setTimeLastChange(date.toString("dd.MM.yyyy hh:mm"));

    return true;
}

bool DslamPortListModel::updatePortMXA(QModelIndex portIndex, QScopedPointer<SnmpClient> &snmp)
{
    netsnmp_variable_list *vars = snmp->varList();

    if (isValidSnmpValue(vars)
            && isValidSnmpValue(vars->next_variable)) {
        QString operStatus = dslamStatePortString(*vars->val.integer);
        QString adminStatus = dslamStatePortString(*vars->next_variable->val.integer);
        QString resultStatus;

        if (adminStatus == "Down")
            resultStatus = "Down";
        else if ((adminStatus == "Up") && (operStatus == "Up"))
            resultStatus = "Up";
        else if ((adminStatus == "Up") && (operStatus == "Down"))
            resultStatus = "Activating";
        else
            resultStatus = "Other";

        mPortList[portIndex.row()]->setState(resultStatus);
    }

    if (mPortList[portIndex.row()]->state() != "Up") {
        std::static_pointer_cast<AdslPortInfo>(mPortList[portIndex.row()])->setTxRate(0);
        std::static_pointer_cast<AdslPortInfo>(mPortList[portIndex.row()])->setRxRate(0);
        mPortList[portIndex.row()]->setTxAttenuation("");
        mPortList[portIndex.row()]->setRxAttenuation("");
        vars = vars->next_variable->next_variable->next_variable->next_variable->next_variable;
    }
    else {
        vars = vars->next_variable->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        std::static_pointer_cast<AdslPortInfo>(mPortList[portIndex.row()])->setTxRate(*vars->val.integer / 1000);

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        std::static_pointer_cast<AdslPortInfo>(mPortList[portIndex.row()])->setRxRate(*vars->val.integer / 1000);

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        mPortList[portIndex.row()]->setTxAttenuation(QString::number(*vars->val.integer));

        vars = vars->next_variable;
        if (!isValidSnmpValue(vars))
            return false;

        mPortList[portIndex.row()]->setRxAttenuation(QString::number(*vars->val.integer));
    }

    vars = vars->next_variable;
    if (!isValidSnmpValue(vars))
        return false;

    mPortList[portIndex.row()]->setProfile(profileExtName(mDeviceModel, QString::number(*vars->val.integer)));

    return true;
}

int DslamPortListModel::currentPort(QModelIndex index)
{
    if (!index.isValid())
        return -1;

    if (index.internalId() == invalidParentIndex) {
        return index.row();
    } else {
        return index.parent().row();
    }
}
