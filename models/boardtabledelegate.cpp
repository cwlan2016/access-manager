#include "boardtabledelegate.h"

#include "boardtablemodel.h"
#include <constant.h>
#include <converters.h>

BoardTableDelegate::BoardTableDelegate(Dslam::Ptr dslamInfo,
                                       QObject *parent) :
    QItemDelegate(parent),
    mDslamInfo(dslamInfo)
{
}

QWidget *BoardTableDelegate::createEditor(QWidget *parent,
                                          const QStyleOptionViewItem &option,
                                          const QModelIndex &index) const
{
    if ((index.column() == BoardTableModel::TypeBoardColumn)
            || (index.column() == BoardTableModel::PairsColumn)) {
        return createComboBoxEditor(parent);
    } else {
        return QItemDelegate::createEditor(parent, option, index);
    }
}

void BoardTableDelegate::setEditorData(QWidget *editor,
                                       const QModelIndex &index) const
{
    if (index.column() == BoardTableModel::TypeBoardColumn) {
        QComboBox *comboBox = qobject_cast<QComboBox *>(editor);

        QString text = index.model()->data(index).toString();

        comboBox->setModel(fillTypeBoardComboBox());

        int indexFind = comboBox->findData(text, Qt::DisplayRole);
        comboBox->setCurrentIndex(indexFind);

        comboBox->setEditable(false);
    } else if (index.column() == BoardTableModel::PairsColumn) {
        QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
        comboBox->setEditable(false);

        QString text = index.model()->data(index).toString();

        QModelIndex indexTypeBoard = index.model()->index(index.row(),
                                                          BoardTableModel::TypeBoardColumn);
        QString typeBoard = index.model()->data(indexTypeBoard).toString();
        comboBox->setModel(fillFirstPairComboBox(typeBoard));

        if (!text.isEmpty()) {
            int indexFind = comboBox->findData(text, Qt::DisplayRole);
            comboBox->setCurrentIndex(indexFind);
        }
    } else {
        QItemDelegate::setEditorData(editor, index);
    }
}

void BoardTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                      const QModelIndex &index) const
{
    if (index.column() == BoardTableModel::TypeBoardColumn) {
        QComboBox *comboBox = qobject_cast<QComboBox *>(editor);

        if (comboBox->currentIndex() == -1)
            return;

        model->setData(index, comboBox->currentText(), Qt::EditRole);
    } else if (index.column() == BoardTableModel::PairsColumn) {
        QComboBox *comboBox = qobject_cast<QComboBox *>(editor);

        if (comboBox->currentIndex() == -1)
            return;

        QString firstPair = comboBox->currentText();

        if (!firstPair.isEmpty()) {
            firstPair = firstPair.split('-').at(0);
        } else {
            return;
        }

        model->setData(index, firstPair, Qt::EditRole);
    } else {
        QItemDelegate::setModelData(editor, model, index);
    }
}

QWidget *BoardTableDelegate::createComboBoxEditor(QWidget *parent) const
{
    QComboBox *editor = new QComboBox(parent);

    connect(editor,
            static_cast<void (QComboBox:: *)(int)>(&QComboBox::currentIndexChanged),
            this,
            &BoardTableDelegate::commitAndCloseComboBoxEditor);

    return editor;
}

void BoardTableDelegate::commitAndCloseComboBoxEditor(int index)
{
    Q_UNUSED(index);

    QComboBox *editor = qobject_cast<QComboBox *>(sender());

    emit commitData(editor);
    emit closeEditor(editor);
}

QStringListModel *BoardTableDelegate::fillTypeBoardComboBox() const
{
    QStringList stringList;

    for (int i = 0; i < BoardType::Count; ++i)
        stringList.push_back(BoardType::BoardTypeName[i]);

    return new QStringListModel(stringList, (QObject *)this);
}

QStringListModel *BoardTableDelegate::fillFirstPairComboBox(QString boardType) const
{
    int countPairs = mDslamInfo->countPorts(BoardType::from(boardType));
    int countBoards = 0;

    if ((mDslamInfo->deviceModel() == DeviceModel::MA5600)
            || (mDslamInfo->deviceModel() == DeviceModel::MA5300)) {
        countBoards = 14;
    }

    QStringList stringList;

    int firstPair = 1;

    for (int i = 0; i < countBoards; ++i) {
        stringList.push_back(QString::fromUtf8("%1-%2")
                             .arg(firstPair)
                             .arg(firstPair + countPairs - 1));
        firstPair += countPairs;
    }

    return new QStringListModel(stringList, (QObject *)this);
}
