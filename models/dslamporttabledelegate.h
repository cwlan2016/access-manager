#ifndef DSLAMPORTTABLEDELEGATE_H
#define DSLAMPORTTABLEDELEGATE_H

#include <stdafx.h>

class DslamPortTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DslamPortTableDelegate(int descriptionPortLength = 0, QObject *parent = 0);
    
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

//    void paint(QPainter *painter, const QStyleOptionViewItem &option,
//               const QModelIndex &index) const;

private:
    int mDescriptionPortLength;
};

#endif // DSLAMPORTTABLEDELEGATE_H
