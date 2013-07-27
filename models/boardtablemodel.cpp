#include "boardtablemodel.h"

#include <constant.h>
#include <converters.h>
#include <customsnmpfunctions.h>
#include <snmpclient.h>

//Columns
//
// 0 - number
// 1 - type_board
// 2 - range pairs

BoardTableModel::BoardTableModel(Dslam *parentDevice, QObject *parent) :
    QAbstractTableModel(parent),
    mParentDevice(parentDevice)
{
}

int BoardTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if (mParentDevice && ((mParentDevice->deviceModel() == DeviceModel::MA5600)
            || (mParentDevice->deviceModel() == DeviceModel::MA5300))) {
        if (!parent.isValid()) {
            return 16;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

int BoardTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 3;
}

QVariant BoardTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole) {
        if (index.column() == 0) {
            return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);
        } else {
            return static_cast<int>(Qt::AlignCenter | Qt::AlignVCenter);
        }
    } else if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (index.column() == 0) {
            return index.row();
        } else if ((index.column() == 1) && mList.contains(index.row())) {
            return BoardType::toString(mList[index.row()]->type());
        } else if ((index.column() == 2) && mList.contains(index.row())) {
            if (role == Qt::DisplayRole) {
                return rangePairs(mList[index.row()]->firstPair(),
                        mList[index.row()]->type());
            } else {
                return mList[index.row()]->firstPair();
            }
        }
    } else if (role == Qt::UserRole) {
        if (index.column() == 1) {
            if (mList.contains(index.row())) {
                return mList[index.row()]->type();
            } else {
                return BoardType::Other;
            }
        }
    }

    return QVariant();
}

bool BoardTableModel::setData(const QModelIndex &index, const QVariant &value,
                              int role)
{
    if (!index.isValid() || (role != Qt::EditRole))
        return false;

    if ((!mList.contains(index.row())) && !value.isNull()) {
        mList.insert(index.row(), new DslamBoard(this));
        mList[index.row()]->setIndex(index.row());
        connect(mList[index.row()], &DslamBoard::modified,
                this, &BoardTableModel::boardIsModified);
    }

    if (index.column() == 1) {
        mList[index.row()]->setType(BoardType::from(value.toString()));
        emit dataChanged(index, index);

        return true;
    } else if (index.column() == 2) {
        mList[index.row()]->setFirstPair(value.toInt());
        emit dataChanged(index, index);

        return true;
    }

    return false;
}

QVariant BoardTableModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if (orientation == Qt::Vertical)
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (section == 0) {
            return BoardTableModelStrings::Number;
        } else if (section == 1) {
            return BoardTableModelStrings::TypeBoard;
        } else if (section == 2) {
            return BoardTableModelStrings::Pairs;
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

Qt::ItemFlags BoardTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    //for MA5600, MA5300 edit all data, without 0 column and 7,8 row
    if (mParentDevice && ((mParentDevice->deviceModel() == DeviceModel::MA5600)
            || (mParentDevice->deviceModel() == DeviceModel::MA5300))) {
        if (mParentDevice->autoFill() && mParentDevice->autoNumeringBoard())
            return flags;

        if (mParentDevice->autoFill() && !mParentDevice->autoNumeringBoard()) {
            if ((index.column() == 2)
                    && (index.row() != 7)
                    && (index.row() != 8)) {
                flags |= Qt::ItemIsEditable;
            }
        } else if ((index.column() != 0)
                   && (index.row() != 7)
                   && (index.row() != 8)) {
            flags |= Qt::ItemIsEditable;
        }
    }

    return flags;
}

bool BoardTableModel::removeRow(int row, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row);

    if (mList.contains(row))
        mList.remove(row);

    endRemoveRows();

    boardIsModified();

    return true;
}

QHash<int, DslamBoard::Ptr> BoardTableModel::boardList() const
{
    return mList;
}

void BoardTableModel::addBoard(int index, BoardType::Enum type,
                               int firstPair)
{
    DslamBoard::Ptr board = newBoard(index, type, firstPair);
    mList.insert(index, board);
}

DslamBoard::Ptr BoardTableModel::newBoard(int index, BoardType::Enum type, int firstPair)
{
    DslamBoard::Ptr board = new DslamBoard(this);
    board->setIndex(index);
    board->setType(type);
    board->setFirstPair(firstPair);
    connect(board, &DslamBoard::modified,
            this, &BoardTableModel::boardIsModified);

    return board;
}

Dslam *BoardTableModel::parentDevice()
{
    return mParentDevice;
}

bool BoardTableModel::getBoardListFromDevice()
{
    if (!mParentDevice)
        return false;
    if ((mParentDevice->deviceModel() != DeviceModel::MA5600)
            && (mParentDevice->deviceModel() != DeviceModel::MA5300)) {
        mError = QString::fromUtf8("%1 %2(%3) не поддерживает автоматическое обновление досок.")
                .arg(DeviceModel::toString(mParentDevice->deviceModel()))
                .arg(mParentDevice->name())
                .arg(mParentDevice->ip());
        return false;
    }

    if (mParentDevice->autoFill() == 0) {
        mError = QString::fromUtf8("На %1 %2(%3) отключено автоматическое обновление досок.")
                .arg(DeviceModel::toString(mParentDevice->deviceModel()))
                .arg(mParentDevice->name())
                .arg(mParentDevice->ip());
        return false;
    }

    QScopedPointer<SnmpClient> snmp(new SnmpClient());

    snmp->setIp(mParentDevice->ip());

    if (!snmp->setupSession(SessionType::ReadSession)) {
        mError = QString::fromUtf8("%1 %2(%3): %4")
                .arg(DeviceModel::toString(mParentDevice->deviceModel()))
                .arg(mParentDevice->name())
                .arg(mParentDevice->ip())
                .arg(SnmpErrorStrings::SetupSession);

        return false;
    }

    if (!snmp->openSession()) {
        mError = QString::fromUtf8("%1 %2(%3): %4")
                .arg(DeviceModel::toString(mParentDevice->deviceModel()))
                .arg(mParentDevice->name())
                .arg(mParentDevice->ip())
                .arg(SnmpErrorStrings::OpenSession);

        return false;
    }

    snmp->createPdu(SNMP_MSG_GETBULK, 16);

    snmp->addOid(createOidPair(Mib::hwSlotType, 14));

    QHash<int, DslamBoard::Ptr> newList;

    if (snmp->sendRequest()) {
        for (auto vars = snmp->varList(); vars;
                vars = vars->next_variable) {
            if (!isValidSnmpValue(vars)) {
                mError = QString::fromUtf8("%1 %2(%3): %4")
                        .arg(DeviceModel::toString(mParentDevice->deviceModel()))
                        .arg(mParentDevice->name())
                        .arg(mParentDevice->ip())
                        .arg(SnmpErrorStrings::Parse);
                return false;
            }

            if (snmp_oid_ncompare(Mib::hwSlotType, 14,
                                  vars->name, vars->name_length, 14) != 0)
                break;

            int slotType = *vars->val.integer;
            int boardIndex = vars->name[14];

            if ((boardIndex == 7) || (boardIndex == 8))
                continue;

            DslamBoard::Ptr board = newBoard(boardIndex, HuaweiSlotType::toBoardType(slotType), 1);
            newList.insert(boardIndex, board);
        }
    } else {
        mError = QString::fromUtf8("При получении данных с %1 %2(%3) произошла ошибка: %4")
                .arg(DeviceModel::toString(mParentDevice->deviceModel()))
                .arg(mParentDevice->name())
                .arg(mParentDevice->ip())
                .arg(snmp->error());

        return false;
    }

    beginResetModel();

    qDeleteAll(mList.begin(), mList.end());
    mList.clear();

    auto end = newList.cend();
    for (auto begin = newList.cbegin(); begin != end; ++begin) {
        mList.insert(begin.key(), begin.value());
    }

    if (mParentDevice && mParentDevice->autoNumeringBoard())
        mParentDevice->boardTableModel()->renumeringPairList();

    endResetModel();

    boardIsModified();

    return true;
}

void BoardTableModel::renumeringPairList()
{
    if (!mParentDevice)
        return;

    int adslStep = mParentDevice->countPorts(BoardType::AnnexA);
    int shdslStep = mParentDevice->countPorts(BoardType::Shdsl);

    int firstAdslPair = 1;
    int firstShdslPair = 1;

    for (int i = 0; i < 16; ++i) {
        if ((i == 7) || (i == 8))
            continue;

        if (!mList.contains(i))
            continue;

        if ((mList[i]->type() == BoardType::AnnexA)
                || (mList[i]->type() == BoardType::AnnexB)) {
            mList[i]->setFirstPair(firstAdslPair);
            firstAdslPair += adslStep;
        } else if (mList[i]->type() == BoardType::Shdsl) {
            mList[i]->setFirstPair(firstShdslPair);
            firstShdslPair += shdslStep;
        }
    }

    QModelIndex first = index(0, 2);
    QModelIndex last = index(mList.count() - 1, 2);
    emit dataChanged(first, last);
}

QString BoardTableModel::error() const
{
    return mError;
}

QString BoardTableModel::rangePairs(int firstPair, BoardType::Enum typeBoard) const
{
    if (!mParentDevice)
        return "";

    return QString("%1-%2")
           .arg(firstPair)
            .arg(firstPair + mParentDevice->countPorts(typeBoard) - 1);
}

void BoardTableModel::boardIsModified()
{
    emit modified();
}

