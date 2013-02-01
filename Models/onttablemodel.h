#ifndef ONTTABLEMODEL_H
#define ONTTABLEMODEL_H

#include <QtCore/QAbstractTableModel>
#ifdef _MSC_VER
#include "../Info/ontinfo.h"
#include "../Info/oltinfo.h"
#else
#include "Info/ontinfo.h"
#include "Info/oltinfo.h"
#endif

class OntTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit OntTableModel(OltInfo::Ptr oltInfo, QObject *parent = 0);
    int rowCount(const QModelIndex &parentDevice) const;
    int columnCount(const QModelIndex &parentDevice) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool load();
    bool updateOntInfo(QString id);
    QString error() const;
private:
    QVector<OntInfo::Ptr> mList;
    QString mError;
    QString mIp;
    OltInfo::Ptr mOltInfo;
};

#endif // ONTTABLEMODEL_H
