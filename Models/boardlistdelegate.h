#ifndef BOARDLISTDELEGATE_H
#define BOARDLISTDELEGATE_H

#include "converters.h"

class BoardListDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    BoardListDelegate(DeviceModel deviceModel, QObject* parent = 0);
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    int indexTypeBoard();
    int indexFirstPair();
    void setIndexTypeBoard(int index);
    void setIndexFirstPair(int index);
private slots:
    void commitAndCloseComboBoxEditor(int);
private:
    int mIndexTypeBoard;
    int mIndexFirstPair;
    DeviceModel mDeviceModel;
    QWidget* createComboBoxEditor(QWidget* parent) const;
    QStringListModel* fillTypeBoardComboBox() const;
    QStringListModel* fillFirstPairComboBox(DeviceModel deviceModel, QString boardType) const;
};

#endif // BOARDLISTDELEGATE_H
