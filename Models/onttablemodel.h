#ifndef ONTTABLEMODEL_H
#define ONTTABLEMODEL_H

#include "stdafx.h"
#include "constant.h"
#include "Info/ontinfo.h"
#include "Info/nteinfo.h"
#include "Info/ntpinfo.h"
#include "Info/oltinfo.h"

class OntTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit OntTableModel(OltInfo::Ptr oltInfo, QObject* parent = 0);
    int rowCount(const QModelIndex& parentDevice) const;
    int columnCount(const QModelIndex& parentDevice) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    bool load();
    bool updateOntInfo(QString id);
    //error
    QString error() const;
signals:
    
public slots:
private:
    std::vector<OntInfo::Ptr> mOntList;
    QString mError;
    QString mIp;
    OltInfo::Ptr mOltInfo;
};

#endif // ONTTABLEMODEL_H
