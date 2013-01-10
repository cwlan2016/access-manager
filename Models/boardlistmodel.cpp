#include "boardlistmodel.h"

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

BoardListModel::BoardListModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int BoardListModel::rowCount(const QModelIndex &parent) const
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

int BoardListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 3;
}

QVariant BoardListModel::data(const QModelIndex &index, int role) const
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
        else if ((index.column() == 1) && mBoardList.contains(index.row()))
            return BoardType::toString(mBoardList[index.row()].type());
        else if ((index.column() == 2) && mBoardList.contains(index.row())) {
            if (role == Qt::DisplayRole) {
                return rangePairs(mBoardList[index.row()].firstPair(), mBoardList[index.row()].type());
            } else {
                return mBoardList[index.row()].firstPair();
            }
        } else
            return "";
    } else if (role == Qt::UserRole) {
        if (index.column() == 1) {
            if (mBoardList.contains(index.row())) {
                return (short)mBoardList[index.row()].type();
            } else {
                return (short)BoardType::Other;
            }
        } else
            return QVariant();
    } else
        return QVariant();
}


bool BoardListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || (role != Qt::EditRole))
        return false;

    if ((!mBoardList.contains(index.row())) && !value.isNull()) {
        mBoardList.insert(index.row(), BoardInfo());
        mBoardList[index.row()].setNumber(index.row());
    }

    if (index.column() == 1) {
        mBoardList[index.row()].setType(BoardType::from(value.toString()));
        emit dataChanged(index, index);

        return true;
    } else if (index.column() == 2) {
        mBoardList[index.row()].setFirstPair(value.toInt());
        emit dataChanged(index, index);

        return true;
    }

    return false;
}

QVariant BoardListModel::headerData(int section, Qt::Orientation orientation, int role) const
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

Qt::ItemFlags BoardListModel::flags(const QModelIndex &index) const
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

bool BoardListModel::removeRow(int row, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row);

    if (mBoardList.contains(row))
        mBoardList.remove(row);

    endRemoveRows();

    //reset();

    return true;
}

QHash<int, BoardInfo> BoardListModel::boardList() const
{
    return mBoardList;
}

short BoardListModel::autoFill() const
{
    return mAutoFill;
}

short BoardListModel::autoNumeringBoard() const
{
    return mAutoNumeringBoard;
}

DeviceInfo::Ptr BoardListModel::parentDevice()
{
    return mParentDevice;
}

void BoardListModel::setBoardList(QHash<int, BoardInfo> &boardList)
{
    beginResetModel();

    mBoardList.clear();

    QHashIterator<int, BoardInfo> i(boardList);

    while (i.hasNext()) {
        i.next();
        mBoardList.insert(i.key(), i.value());
    }

    endResetModel();
    //reset();
}

void BoardListModel::setAutoFill(short autoFill)
{
    mAutoFill = autoFill;
}

void BoardListModel::setAutoNumeringBoard(short autoNumeringBoard)
{
    mAutoNumeringBoard = autoNumeringBoard;
}

void BoardListModel::setParentDevice(const DeviceInfo::Ptr &parent)
{
    mParentDevice = parent;
}

bool BoardListModel::getBoardListFromDevice()
{
    beginResetModel();

    if (mParentDevice->deviceModel() != DeviceModel::MA5600) {
        mError = QString::fromUtf8("Автоматическое обновление списка досок невозможно для данной модели дслама.");
        endResetModel();
        return false;
    }

    if (mAutoFill == 0) {
        mError = QString::fromUtf8("На данном дсламе отключено автоматическое обновление досок.");
        endResetModel();
        return false;
    }

    QScopedPointer<SnmpClient> snmp(new SnmpClient());

    snmp->setIP(mParentDevice->ip());

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

    snmp->createPdu(SNMP_MSG_GETBULK, 16);

    snmp->addOid(Mib::dslamBoardName, 15);


    if (snmp->sendRequest()) {
        mBoardList.clear();

        oid *boardNameOid = CreateOid(Mib::dslamBoardName, 15); //[15];
        size_t lenBoardNameOid = 15;
        //snmp_parse_oid(Mib::dslamBoardName.toLatin1().data(), boardNameOid, &lenBoardNameOid);

        for (auto vars = snmp->varList(); vars;
                vars = vars->next_variable) {
            if (!IsValidSnmpValue(vars)) {
                mError = SnmpErrors::GetInfo;
                endResetModel();
                return false;
            }

            if (snmp_oid_ncompare(boardNameOid, lenBoardNameOid, vars->name, vars->name_length, 15) != 0)
                break;

            QString boardName = QString::fromLatin1(reinterpret_cast<char *>(vars->val.string), vars->val_len);
            int boardIndex = vars->name[16];

            //если управляющая доска, то пропускаем
            if ((boardIndex == 7) || (boardIndex == 8))
                continue;

            BoardInfo info;
            info.setNumber(boardIndex);
            info.setType(boardTypeFromBoardName(boardName));
            mBoardList.insert(boardIndex, info);
        }

    } else {
        mError = SnmpErrors::GetInfo;
        endResetModel();
        return false;
    }

    endResetModel();
    //reset();

    return true;
}

void BoardListModel::renumeringPairList()
{
    //beginResetModel();

    int adslStep = countPorts(mParentDevice->deviceModel(), BoardType::AnnexA);
    int shdslStep = countPorts(mParentDevice->deviceModel(), BoardType::Shdsl);

    int firstAdslPair = 1;
    int firstShdslPair = 1;

    for (int i = 0; i < 16; ++i) {
        if ((i == 7) || (i == 8))
            continue;

        if (!mBoardList.contains(i))
            continue;

        if ((mBoardList[i].type() == BoardType::AnnexA) || (mBoardList[i].type() == BoardType::AnnexB)) {
            mBoardList[i].setFirstPair(firstAdslPair);
            firstAdslPair += adslStep;
        } else if (mBoardList[i].type() == BoardType::Shdsl) {
            mBoardList[i].setFirstPair(firstShdslPair);
            firstShdslPair += shdslStep;
        }
    }

    QModelIndex first = index(0, 2);
    QModelIndex last = index(mBoardList.count() - 1, 2);
    emit dataChanged(first, last);

    //endResetModel();
    //reset();
}

QString BoardListModel::error() const
{
    return mError;
}

QString BoardListModel::rangePairs(int firstPair, BoardType::Enum typeBoard) const
{
    return QString("%1-%2")
           .arg(firstPair)
           .arg(firstPair + countPorts(mParentDevice->deviceModel(), typeBoard) - 1);
}
