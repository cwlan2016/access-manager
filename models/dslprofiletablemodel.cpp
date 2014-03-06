#include "dslprofiletablemodel.h"

#include <constant.h>

DslProfileTableModel::DslProfileTableModel(QList<DslProfile> *list,
                                           QObject *parent) :
    QAbstractTableModel(parent)
{
    mCountDefaultProfiles = 0;

    if (list) {
        mCountDefaultProfiles = list->size();

        auto it = list->begin();
        auto end = list->end();

        for (; it != end; ++it) {
            mList.push_back(qMakePair((*it).first, (*it).second));
        }

        delete list;
    }
}

int DslProfileTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return mList.size();
}

int DslProfileTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 2;
}

QVariant DslProfileTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole) {
            return int(Qt::AlignCenter | Qt::AlignVCenter);
    } else if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (index.column() == DisplayNameColumn) {
            return mList.at(index.row()).first;
        } else if (index.column() == DslamNameColumn) {
            return mList.at(index.row()).second;
        }
    } else if (role == Qt::FontRole) {
        if (index.row() >= mCountDefaultProfiles)
            return false;

        QFont font(qApp->font());
        font.setBold(true);
        return font;
    }

    return QVariant();
}

bool DslProfileTableModel::setData(const QModelIndex &index,
                                   const QVariant &value, int role)
{
    if (!index.isValid() || (role != Qt::EditRole))
        return false;

    if (role == Qt::EditRole) {
        if (index.column() == DisplayNameColumn) {
            mList[index.row()].first = value.toString();
            emit dataChanged(index, index);

            return true;
        } else if (index.column() == DslamNameColumn) {
            mList[index.row()].second = value.toString();
            emit dataChanged(index, index);

            return true;
        }
    }

    return false;
}

QVariant DslProfileTableModel::headerData(int section,
                                          Qt::Orientation orientation,
                                          int role) const
{
    if (orientation == Qt::Vertical)
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (section == DisplayNameColumn) {
            return DslProfileTableModelStrings::DisplayName;
        } else if (section == DslamNameColumn) {
            return DslProfileTableModelStrings::DslamName;
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

Qt::ItemFlags DslProfileTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (index.row() >= mCountDefaultProfiles)
        flags |= Qt::ItemIsEditable;

    return flags;
}

bool DslProfileTableModel::insertRow(int row, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row);
    mList.push_back(qMakePair(QString(""), QString("")));
    endInsertRows();

    return true;
}

bool DslProfileTableModel::removeRow(int row, const QModelIndex &parent)
{
    if (row < mCountDefaultProfiles)
        return false;

    beginRemoveRows(parent, row, row);
    mList.erase(mList.begin() + row);
    endRemoveRows();

    return true;
}

QString DslProfileTableModel::dslamProfileName(int index)
{
    if ((index < 0) || (index > mList.size() - 1))
        return "";

    return mList.at(index).second;
}

QString DslProfileTableModel::displayProfileName(QString dslamName)
{

    auto end = mList.constEnd();

    for (auto it = mList.constBegin(); it != end; ++it) {
        if ((*it).second == dslamName)
            return (*it).first;
    }

    return QString("Other(%1)").arg(dslamName);
}

QString DslProfileTableModel::configString()
{
    QString result = "";

    int size = mList.size();
    for (int i = mCountDefaultProfiles; i < size; ++i) {
        result += QString("%1|%2|").arg(mList[i].first).arg(mList[i].second);
    }

    if (!result.isEmpty())
        result.remove(result.size() - 1, 1);

    return result;
}

void DslProfileTableModel::fromConfigString(QString value)
{
    QStringList list = value.split('|');

    if (list.size() % 2 != 0)
        return;

    int size = list.size();
    for (int i = 0; i < size; i += 2) {
        mList.push_back(qMakePair(list.at(i), list.at(i + 1)));
    }
}
