#include "devicetabledelegate.h"

#include <constant.h>
#include "devicetablemodel.h"

DeviceTableDelegate::DeviceTableDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *DeviceTableDelegate::createEditor(QWidget *parent,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const
{
    if (index.column() == DeviceTableModel::DeviceModelColumn) {
        QComboBox *editor = new QComboBox(parent);
        editor->setMinimumWidth(100);

        return editor;
    } else {
        return QItemDelegate::createEditor(parent, option, index);
    }
}

void DeviceTableDelegate::setEditorData(QWidget *editor,
                                        const QModelIndex &index) const
{
    if (index.column() == DeviceTableModel::DeviceModelColumn) {
        QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
        QString text = index.model()->data(index).toString();

        comboBox->setModel(fillDeviceModelComboBox());

        int indexFind = comboBox->findData(text, Qt::DisplayRole);
        comboBox->setCurrentIndex(indexFind);

        comboBox->setEditable(false);
    } else {
        QItemDelegate::setEditorData(editor, index);
    }
}

void DeviceTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                      const QModelIndex &index) const
{
    if (index.column() == DeviceTableModel::DeviceModelColumn) {
        QComboBox *comboBox = qobject_cast<QComboBox *>(editor);

        if (comboBox->currentIndex() == -1)
            return;

        model->setData(index, comboBox->currentText(), Qt::EditRole);
    } else {
        QItemDelegate::setModelData(editor, model, index);
    }
}

QStringListModel *DeviceTableDelegate::fillDeviceModelComboBox() const
{
    QStringList stringList;

    for (int i = 0; i < DeviceModel::Count; ++i)
        stringList.push_back(DeviceModel::DeviceModelName[i]);

    return new QStringListModel(stringList, (QObject *)this);
}
