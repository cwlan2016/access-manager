#include "boardlistdelegate.h"

#include <QtWidgets/QComboBox>

#ifdef _MSC_VER
#include "../constant.h"
#include "../converters.h"
#else
#include "constant.h"
#include "converters.h"
#endif

BoardListDelegate::BoardListDelegate(DeviceModel::Enum deviceModel, QObject *parent) :
    QItemDelegate(parent)
{
    mDeviceModel = deviceModel;
}

QWidget *BoardListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if ((index.column() == mIndexTypeBoard) || (index.column() == mIndexFirstPair)) {
        return createComboBoxEditor(parent);
    } else {
        //Делегат по умолчанию
        return QItemDelegate::createEditor(parent, option, index);
    }
}

void BoardListDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.column() == mIndexTypeBoard) {
        QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
        //запоминаем текст от предыдущего редактирвоания
        QString text = index.model()->data(index).toString();

        comboBox->setModel(fillTypeBoardComboBox());
        //ищем запомненный итем в новой коллекции
        int indexFind = comboBox->findData(text, Qt::DisplayRole);
        comboBox->setCurrentIndex(indexFind);

        comboBox->setEditable(false);
    } else if (index.column() == mIndexFirstPair) {
        QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
        comboBox->setEditable(false);
        //запоминаем текст от предыдущего редактирования
        QString text = index.model()->data(index).toString();

        QModelIndex indexTypeBoard = index.model()->index(index.row(), mIndexTypeBoard);
        QString typeBoard = index.model()->data(indexTypeBoard).toString();
        comboBox->setModel(fillFirstPairComboBox(mDeviceModel, typeBoard));

        //ищем запомненный итем в новой коллекции
        if (!text.isEmpty()) {
            int indexFind = comboBox->findData(text, Qt::DisplayRole);
            comboBox->setCurrentIndex(indexFind);
        }
    } else {
        //Делегат по умолчанию
        QItemDelegate::setEditorData(editor, index);
    }
}

void BoardListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (index.column() == mIndexTypeBoard) {
        QComboBox *comboBox = qobject_cast<QComboBox *>(editor);

        if (comboBox->currentIndex() == -1)
            return;

        model->setData(index, comboBox->currentText(), Qt::EditRole);
    } else if (index.column() == mIndexFirstPair) {
        QComboBox *comboBox = qobject_cast<QComboBox *>(editor);

        if (comboBox->currentIndex() == -1)
            return;

        QString firstPair = comboBox->currentText();

        if (!firstPair.isEmpty())
            firstPair = firstPair.split("-").at(0);
        else
            return;

        model->setData(index, firstPair, Qt::EditRole);
    } else {
        //Делегат по умолчанию
        QItemDelegate::setModelData(editor, model, index);
    }
}

int BoardListDelegate::indexTypeBoard()
{
    return mIndexTypeBoard;
}

int BoardListDelegate::indexFirstPair()
{
    return mIndexFirstPair;
}

void BoardListDelegate::setIndexTypeBoard(int index)
{
    mIndexTypeBoard = index;
}

void BoardListDelegate::setIndexFirstPair(int index)
{
    mIndexFirstPair = index;
}

void BoardListDelegate::commitAndCloseComboBoxEditor(int index)
{
    Q_UNUSED(index);

    QComboBox *editor = qobject_cast<QComboBox *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

QWidget *BoardListDelegate::createComboBoxEditor(QWidget *parent) const
{
    QComboBox *editor = new QComboBox(parent);
    connect(editor, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &BoardListDelegate::commitAndCloseComboBoxEditor);
    return editor;
}

QStringListModel *BoardListDelegate::fillTypeBoardComboBox() const
{
    QStringList stringList;

    for (int i = 0; i < BoardType::Count; ++i) {
        stringList.push_back(BoardType::BoardTypeName[i]);
    }

    return new QStringListModel(stringList, (QObject *)this);
}

QStringListModel *BoardListDelegate::fillFirstPairComboBox(DeviceModel::Enum deviceModel, QString boardType) const
{
    int countPairs = CountPorts(deviceModel, BoardType::from(boardType));
    int countBoards = 0;

    if ((deviceModel == DeviceModel::MA5600)
            || (deviceModel == DeviceModel::MA5300)) {
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
