#include "devicelistdelegate.h"

DeviceListDelegate::DeviceListDelegate(QObject* parent) :
    QItemDelegate(parent)
{
}

QWidget* DeviceListDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if(index.column() == mIndexDeviceModel)
    {
        return createComboBoxEditor(parent);
    }
    else
    {
        //Делегат по умолчанию
        return QItemDelegate::createEditor(parent, option, index);
    }
}

void DeviceListDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    if(index.column() == mIndexDeviceModel)
    {
        QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
        //запоминаем текст от предыдущего редактирования
        QString text = index.model()->data(index).toString();

        comboBox->setModel(fillDeviceModelComboBox());
        //ищем запомненный итем в новой коллекции
        int indexFind = comboBox->findData(text, Qt::DisplayRole);
        comboBox->setCurrentIndex(indexFind);

        comboBox->setEditable(false);
    }
    else
    {
        //Делегат по умолчанию
        QItemDelegate::setEditorData(editor, index);
    }
}

void DeviceListDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    if(index.column() == mIndexDeviceModel)
    {
        QComboBox* comboBox = qobject_cast<QComboBox*>(editor);

        if(comboBox->currentIndex() == -1)
            return;

        model->setData(index, comboBox->currentText(), Qt::EditRole);
    }
    else
    {
        //Делегат по умолчанию
        QItemDelegate::setModelData(editor, model, index);
    }
}

void DeviceListDelegate::commitAndCloseComboBoxEditor(int)
{
    QComboBox* editor = qobject_cast<QComboBox*>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

int DeviceListDelegate::indexDeviceModel()
{
    return mIndexDeviceModel;
}

void DeviceListDelegate::setIndexDeviceModel(int index)
{
    mIndexDeviceModel = index;
}

QStringListModel* DeviceListDelegate::fillDeviceModelComboBox() const
{
    QStringList stringList;

for(const QString deviceModel : DeviceModelName)
        stringList << deviceModel;

    return new QStringListModel(stringList);
}

QWidget* DeviceListDelegate::createComboBoxEditor(QWidget* parent) const
{
    QComboBox* editor = new QComboBox(parent);
    connect(editor, SIGNAL(currentIndexChanged(int)), this, SLOT(commitAndCloseComboBoxEditor(int)));
    editor->setMinimumWidth(100);
    return editor;
}

