#include "dslamporttabledelegate.h"

#include <constant.h>
#include "dslamporttablemodel.h"

DslamPortTableDelegate::DslamPortTableDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget *DslamPortTableDelegate::createEditor(QWidget *parent,
                                              const QStyleOptionViewItem &option,
                                              const QModelIndex &index) const
{
    if ((index.internalId() != invalidParentIndex)
            || (index.column() != DslamPortTableModel::DescColumn))
        return QStyledItemDelegate::createEditor(parent, option, index);

    QLineEdit *editor = new QLineEdit(parent);

    //all ascii symbols. max length string = mDescriptionPortLength
    QRegExp regExp(QString("[\\x0000-\\x007F]{0,%1}").arg(mDescriptionPortLength));

    QRegExpValidator *validator = new QRegExpValidator(regExp, parent);
    editor->setValidator(validator);

    return editor;
}

void DslamPortTableDelegate::setEditorData(QWidget *editor,
                                           const QModelIndex &index) const
{
    if ((index.internalId() != invalidParentIndex)
            || (index.column() != DslamPortTableModel::DescColumn)) {
        QStyledItemDelegate::setEditorData(editor, index);
        return;
    }

    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    lineEdit->setText(index.model()->data(index).toString());
}

void DslamPortTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if ((index.column() == DslamPortTableModel::DescColumn)
            && (index.internalId() == invalidParentIndex)) {
        QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);

        QString text = lineEdit->text();
        if (text.isEmpty())
            text = " ";

        model->setData(index, text, Qt::EditRole);
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void DslamPortTableDelegate::setDescriptionPortLength(int length)
{
    mDescriptionPortLength = length;
}

//void DslamPortTableDelegate::paint(QPainter *painter,
//                                   const QStyleOptionViewItem &option,
//                                   const QModelIndex &index) const
//{
//   QStyledItemDelegate::paint(painter, option, index);
   // return;

//    if (index.internalId() == invalidParentIndex || index.column() != 1) {
//        QStyledItemDelegate::paint(painter, option, index);
//        return;
//    }

    //painter->draw
    //const QWidget *widget = option.widget;
    //QStyle *style = widget ? widget->style() : QApplication::style();
    //style->drawControl(QStyle::CE_ItemViewItem, option, painter, widget);
//}
