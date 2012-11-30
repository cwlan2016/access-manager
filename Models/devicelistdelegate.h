#ifndef DEVICELISTDELEGATE_H
#define DEVICELISTDELEGATE_H

#include "stdafx.h"
#include "constant.h"

class DeviceListDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit DeviceListDelegate(QObject* parent = 0);
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    int indexDeviceModel();
    void setIndexDeviceModel(int index);
    QStringListModel* fillDeviceModelComboBox() const;
private slots:
    void commitAndCloseComboBoxEditor(int);
private:
    int mIndexDeviceModel;

    QWidget* createComboBoxEditor(QWidget* parent) const;
};

#endif // DEVICELISTDELEGATE_H
