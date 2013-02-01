#include "boardtablemodel.h"

#include <QtGui/QFont>
#include <QtWidgets/QApplication>
#ifdef _MSC_VER
#include "../constant.h"
#include "../converters.h"
#include "../customsnmpfunctions.h"
#include "../snmpclient.h"
#else
#include "constant.h"
#include "converters.h"
#include "customsnmpfunctions.h"
#include "snmpclient.h"
#endif

//Columns
//
// 0 - number
// 1 - type_board
// 2 - range pairs

BoardTableModel::BoardTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int BoardTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if ((mParentDevice->deviceModel() == DeviceModel::MA5600)
            || (mParentDevice->deviceModel() == DeviceModel::MA5300)) {
        if (!parent.isValid())
            return 16;
        else
            return 0;
    } else
        return 0;
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
        if (index.column() == 0)
            return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter); //номер слева
        else
            return static_cast<int>(Qt::AlignCenter | Qt::AlignVCenter); //тип и диапозон пар в центре
    } else if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (index.column() == 0)
            return index.row();
        else if ((index.column() == 1) && mList.contains(index.row()))
            return BoardType::toString(mList[index.row()].type());
        else if ((index.column() == 2) && mList.contains(index.row())) {
            if (role == Qt::DisplayRole) {
                return rangePairs(mList[index.row()].firstPair(), mList[index.row()].type());
            } else {
                return mList[index.row()].firstPair();
            }
        } else
            return "";
    } else if (role == Qt::UserRole) {
        if (index.column() == 1) {
            if (mList.contains(index.row())) {
                return (short)mList[index.row()].type();
            } else {
                return (short)BoardType::Other;
            }
        } else
            return QVariant();
    } else
        return QVariant();
}


bool BoardTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || (role != Qt::EditRole))
        return false;

    if ((!mList.contains(index.row())) && !value.isNull()) {
        mList.insert(index.row(), BoardInfo());
        mList[index.row()].setNumber(index.row());
    }

    if (index.column() == 1) {
        mList[index.row()].setType(BoardType::from(value.toString()));
        emit dataChanged(index, index);

        return true;
    } else if (index.column() == 2) {
        mList[index.row()].setFirstPair(value.toInt());
        emit dataChanged(index, index);

        return true;
    }

    return false;
}

QVariant BoardTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical)
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (section == 0)
            return BoardListModelColumn::Number;
        else if (section == 1)
            return BoardListModelColumn::TypeBoard;
        else if (section == 2)
            return BoardListModelColumn::Pairs;
        else
            return "";
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

    //для MA5600, MA5300 редактируются все данные, кроме 0 столбца и 7,8 строк
    if ((mParentDevice->deviceModel() == DeviceModel::MA5600)
            || (mParentDevice->deviceModel() == DeviceModel::MA5300)) {
        if (mAutoFill && mAutoNumeringBoard)
            return flags;

        if (mAutoFill && !mAutoNumeringBoard) {
            if ((index.column() == 2) && (index.row() != 7) && (index.row() != 8)) {
                flags |= Qt::ItemIsEditable;
            }
        } else if ((index.column() != 0) && (index.row() != 7) && (index.row() != 8)) {
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

    return true;
}

QHash<int, BoardInfo> BoardTableModel::boardList() const
{
    return mList;
}

short BoardTableModel::autoFill() const
{
    return mAutoFill;
}

short BoardTableModel::autoNumeringBoard() const
{
    return mAutoNumeringBoard;
}

DeviceInfo::Ptr BoardTableModel::parentDevice()
{
    return mParentDevice;
}

void BoardTableModel::setBoardList(QHash<int, BoardInfo> &boardList)
{
    beginResetModel();

    mList.clear();

    QHashIterator<int, BoardInfo> i(boardList);

    while (i.hasNext()) {
        i.next();
        mList.insert(i.key(), i.value());
    }

    endResetModel();
}

void BoardTableModel::setAutoFill(short autoFill)
{
    mAutoFill = autoFill;
}

void BoardTableModel::setAutoNumeringBoard(short autoNumeringBoard)
{
    mAutoNumeringBoard = autoNumeringBoard;
}

void BoardTableModel::setParentDevice(const DeviceInfo::Ptr &parent)
{
    mParentDevice = parent;
}

bool BoardTableModel::getBoardListFromDevice()
{
    if (mParentDevice->deviceModel() != DeviceModel::MA5600) {
        mError = QString::fromUtf8("Автоматическое обновление списка досок невозможно для данной модели дслама.");
        return false;
    }

    if (mAutoFill == 0) {
        mError = QString::fromUtf8("На данном дсламе отключено автоматическое обновление досок.");
        return false;
    }

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

    snmp->createPdu(SNMP_MSG_GETBULK, 16);

    snmp->addOid(Mib::dslamBoardName, 16);


    if (snmp->sendRequest()) {
        beginResetModel();

        mList.clear();

        for (auto vars = snmp->varList(); vars;
                vars = vars->next_variable) {
            if (!isValidSnmpValue(vars)) {
                mError = SnmpErrors::GetInfo;
                endResetModel();
                return false;
            }

            if (snmp_oid_ncompare(Mib::dslamBoardName, 16, vars->name, vars->name_length, 16) != 0)
                break;

            QString boardName = QString::fromLatin1(reinterpret_cast<char *>(vars->val.string), vars->val_len);
            int boardIndex = vars->name[16];

            //если управляющая доска, то пропускаем
            if ((boardIndex == 7) || (boardIndex == 8))
                continue;

            BoardInfo info;
            info.setNumber(boardIndex);
            info.setType(boardTypeFromBoardName(boardName));
            mList.insert(boardIndex, info);
        }

    } else {
        mError = SnmpErrors::GetInfo;
        endResetModel();
        return false;
    }

    endResetModel();

    return true;
}

void BoardTableModel::renumeringPairList()
{
    int adslStep = countPorts(mParentDevice->deviceModel(), BoardType::AnnexA);
    int shdslStep = countPorts(mParentDevice->deviceModel(), BoardType::Shdsl);

    int firstAdslPair = 1;
    int firstShdslPair = 1;

    for (int i = 0; i < 16; ++i) {
        if ((i == 7) || (i == 8))
            continue;

        if (!mList.contains(i))
            continue;

        if ((mList[i].type() == BoardType::AnnexA) || (mList[i].type() == BoardType::AnnexB)) {
            mList[i].setFirstPair(firstAdslPair);
            firstAdslPair += adslStep;
        } else if (mList[i].type() == BoardType::Shdsl) {
            mList[i].setFirstPair(firstShdslPair);
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
    return QString("%1-%2")
           .arg(firstPair)
           .arg(firstPair + countPorts(mParentDevice->deviceModel(), typeBoard) - 1);
}
