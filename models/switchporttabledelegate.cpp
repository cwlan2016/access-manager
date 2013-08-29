#include "switchporttabledelegate.h"

#include "switchporttablemodel.h"

SwitchPortTableDelegate::SwitchPortTableDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *SwitchPortTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() != SwitchPortTableModel::DescColumn)
        return QItemDelegate::createEditor(parent, option, index);

    QLineEdit *editor = new QLineEdit(parent);

    //all ascii symbols. max length string = mDescriptionPortLength
    QRegExp regExp(QString("[\\x0000-\\x007F]{0,%1}").arg(mDescriptionPortLength));

    QRegExpValidator *validator = new QRegExpValidator(regExp, parent);
    editor->setValidator(validator);

    return editor;
}

void SwitchPortTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.column() != SwitchPortTableModel::DescColumn) {
        QItemDelegate::setEditorData(editor, index);
        return;
    }

    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    lineEdit->setText(index.model()->data(index).toString());
}

void SwitchPortTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (index.column() == SwitchPortTableModel::DescColumn) {
        QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);

        QString text = lineEdit->text();
        if (text.isEmpty())
            text = " ";

        model->setData(index, text, Qt::EditRole);
    } else {
        QItemDelegate::setModelData(editor, model, index);
    }
}

void SwitchPortTableDelegate::setDescriptionPortLength(int length)
{
    mDescriptionPortLength = length;
}
