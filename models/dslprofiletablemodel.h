#ifndef DSLPROFILETABLEMODEL_H
#define DSLPROFILETABLEMODEL_H

#include <stdafx.h>
#include <customtypes.h>
#include <devices/dslam.h>

class DslProfileTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum ColumnIndex {
        DisplayNameColumn = 0,
        DslamNameColumn
    };

    explicit DslProfileTableModel(QList<DslProfile> *list, QObject *parent = 0);
    
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool insertRow(int row, const QModelIndex &parent);
    bool removeRow(int row, const QModelIndex &parent);

    QString dslamProfileName(int index);
    QString displayProfileName(QString dslamName);

    QString configString();
    void fromConfigString(QString value);

private:
    QList<DslProfile> mList;
    int mCountDefaultProfiles;
};

#endif // DSLPROFILETABLEMODEL_H
