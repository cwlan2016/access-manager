#include "onttablemodel.h"

#ifdef _MSC_VER
#include "../constant.h"
#include "../Info/nteinfo.h"
#include "../Info/ntpinfo.h"
#else
#include "constant.h"
#include "Info/nteinfo.h"
#include "Info/ntpinfo.h"
#endif

OntTableModel::OntTableModel(OltInfo::Ptr oltInfo, QObject *parent) :
    QAbstractTableModel(parent),
    mOltInfo(oltInfo)
{
}

int OntTableModel::rowCount(const QModelIndex &parentDevice) const
{
    Q_UNUSED(parentDevice)

    return mList.size();
}

int OntTableModel::columnCount(const QModelIndex &parentDevice) const
{
    Q_UNUSED(parentDevice)

    return 6;
}

QVariant OntTableModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(index);
    Q_UNUSED(role);

    return QVariant();
}

QVariant OntTableModel::headerData(int section, Qt::Orientation orientation,
                                   int role) const
{
    if (orientation == Qt::Vertical)
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (section == 0) {
            return OntTableModelStrings::Id;
        } else if (section == 1) {
            return OntTableModelStrings::Model;
        } else if (section == 2) {
            return OntTableModelStrings::State;
        } else if (section == 3) {
            return OntTableModelStrings::Description;
        } else if (section == 4) {
            return OntTableModelStrings::ServiceProfile;
        } else if (section == 5) {
            return OntTableModelStrings::MulticastProfile;
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

Qt::ItemFlags OntTableModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool OntTableModel::load()
{
    return false;
}

bool OntTableModel::updateOntInfo(QString id)
{
    Q_UNUSED(id)

    return false;
}

QString OntTableModel::error() const
{
    return mError;
}
