#ifndef BOARDLISTDELEGATE_H
#define BOARDLISTDELEGATE_H

#include <QtCore/QStringListModel>
#include <QtWidgets/QItemDelegate>

#ifdef _MSC_VER
#include "../customtypes.h"
#else
#include "customtypes.h"
#endif

class BoardListDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    BoardListDelegate(DeviceModel::Enum deviceModel, QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    int indexTypeBoard();
    int indexFirstPair();
    void setIndexTypeBoard(int index);
    void setIndexFirstPair(int index);
private:
    void commitAndCloseComboBoxEditor(int index);
    QWidget *createComboBoxEditor(QWidget *parent) const;
    QStringListModel *fillTypeBoardComboBox() const;
    QStringListModel *fillFirstPairComboBox(DeviceModel::Enum deviceModel, QString boardType) const;

    int mIndexTypeBoard;
    int mIndexFirstPair;
    DeviceModel::Enum mDeviceModel;
};

#endif // BOARDLISTDELEGATE_H
