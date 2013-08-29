#include "devicetabledelegate.h"

#include <constant.h>

DeviceTableDelegate::DeviceTableDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *DeviceTableDelegate::createEditor(QWidget *parent,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const
{
    if (index.column() == mIndexDeviceModel) {
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
    if (index.column() == mIndexDeviceModel) {
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
    if (index.column() == mIndexDeviceModel) {
        QComboBox *comboBox = qobject_cast<QComboBox *>(editor);

        if (comboBox->currentIndex() == -1)
            return;

        model->setData(index, comboBox->currentText(), Qt::EditRole);
    } else {
        QItemDelegate::setModelData(editor, model, index);
    }
}

int DeviceTableDelegate::indexDeviceModel()
{
    return mIndexDeviceModel;
}

void DeviceTableDelegate::setIndexDeviceModel(int index)
{
    mIndexDeviceModel = index;
}

QStringListModel *DeviceTableDelegate::fillDeviceModelComboBox() const
{
    QStringList stringList;

    for (int i = 0; i < DeviceModel::Count; ++i)
        stringList.push_back(DeviceModel::DeviceModelName[i]);

    return new QStringListModel(stringList, (QObject *)this);
}
