#ifndef DEVICETABLEDELEGATE_H
#define DEVICETABLEDELEGATE_H

#include <QtCore/QStringListModel>
#include <QtWidgets/QItemDelegate>
#ifdef _MSC_VER
#include "../customtypes.h"
#else
#include "customtypes.h"
#endif

class DeviceTableDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit DeviceTableDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    int indexDeviceModel();
    void setIndexDeviceModel(int index);
    QStringListModel *fillDeviceModelComboBox() const;
private:
    void commitAndCloseComboBoxEditor(int index);
    QWidget *createComboBoxEditor(QWidget *parent) const;

    int mIndexDeviceModel;
};

#endif // DEVICETABLEDELEGATE_H
