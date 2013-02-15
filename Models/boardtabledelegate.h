#ifndef BOARDTABLEDELEGATE_H
#define BOARDTABLEDELEGATE_H

#ifdef _MSC_VER
#include "../customtypes.h"
#else
#include "customtypes.h"
#endif

class BoardTableDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    BoardTableDelegate(DeviceModel::Enum deviceModel, QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    int indexTypeBoard();
    void setIndexTypeBoard(int index);

    int indexFirstPair();
    void setIndexFirstPair(int index);

private:
    QWidget *createComboBoxEditor(QWidget *parent) const;
    void commitAndCloseComboBoxEditor(int index);

    QStringListModel *fillTypeBoardComboBox() const;
    QStringListModel *fillFirstPairComboBox(DeviceModel::Enum deviceModel,
                                            QString boardType) const;

    int mIndexTypeBoard;
    int mIndexFirstPair;
    DeviceModel::Enum mDeviceModel;
};

#endif // BOARDTABLEDELEGATE_H
