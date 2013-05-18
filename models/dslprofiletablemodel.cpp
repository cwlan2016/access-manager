#include "dslprofiletablemodel.h"

#include <constant.h>

DslProfileTableModel::DslProfileTableModel(QList<DslProfile> *list,
                                           QObject *parent) :
    QAbstractTableModel(parent)
{
    countDefaultProfiles = 0;
    if (list) {
        countDefaultProfiles = list->size();

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
        if (index.column() == 0) {
            return mList.at(index.row()).first;
        } else if (index.column() == 1) {
            return mList.at(index.row()).second;
        }
    }

    return QVariant();
}

bool DslProfileTableModel::setData(const QModelIndex &index,
                                   const QVariant &value, int role)
{
    if (!index.isValid() || (role != Qt::EditRole))
        return false;

    if (role == Qt::EditRole) {
        if (index.column() == 0) {
            mList[index.row()].first = value.toString();
            emit dataChanged(index, index);

            return true;
        } else if (index.column() == 1) {
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
        if (section == 0) {
            return DslProfileTableModelStrings::DisplayName;
        } else if (section == 1) {
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

    return Qt::ItemIsEditable | flags;
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
