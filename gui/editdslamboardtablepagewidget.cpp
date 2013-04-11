#include "editdslamboardtablepagewidget.h"
#include "ui_editdslamboardtablepagewidget.h"

#ifdef _MSC_VER
#include "../basicdialogs.h"
#include "../constant.h"
#include "../devices/dslam.h"
#include "../models/boardtabledelegate.h"
#include "../models/boardtablemodel.h"
#else
#include "basicdialogs.h"
#include "constant.h"
#include "devices/dslam.h"
#include "models/boardtabledelegate.h"
#include "models/boardtablemodel.h"
#endif

EditDslamBoardTablePageWidget::EditDslamBoardTablePageWidget(Device::Ptr deviceInfo,
                                                             DeviceTableModel *deviceListModel,
                                                             QWidget *parent) :
    PageWidget(deviceInfo, parent),
    ui(new Ui::EditDslamBoardTablePageWidget),
    mDeviceTableModel(deviceListModel)
{
    ui->setupUi(this);

    connect(ui->editBoardAction, &QAction::triggered,
            this, &EditDslamBoardTablePageWidget::editBoard);
    connect(ui->removeBoardAction, &QAction::triggered,
            this, &EditDslamBoardTablePageWidget::removeBoard);
    connect(ui->getBoardListAction, &QAction::triggered,
            this, &EditDslamBoardTablePageWidget::getBoardList);
    connect(ui->renumeringBoardPairsAction, &QAction::triggered,
            this, &EditDslamBoardTablePageWidget::renumeringBoardPairs);

    Dslam::Ptr dslamInfo = mDeviceInfo.objectCast<Dslam>();
    BoardTableModel *boardTableModel = dslamInfo->boardTableModel();

    ui->editDslamBoardListTableView->setModel(boardTableModel);

    BoardTableDelegate *boardListDelegate = new BoardTableDelegate(mDeviceInfo.objectCast<Dslam>(), this);
    boardListDelegate->setIndexTypeBoard(1);
    boardListDelegate->setIndexFirstPair(2);

    ui->editDslamBoardListTableView->setItemDelegate(boardListDelegate);

    ui->autoUpdateBoardListCheckBox->setCheckState(dslamInfo->autoFill() == 1 ? Qt::Checked : Qt::Unchecked);
    ui->autoNumeringPairCheckBox->setEnabled(dslamInfo->autoFill());
    ui->autoNumeringPairCheckBox->setCheckState(dslamInfo->autoNumeringBoard() == 1 ? Qt::Checked : Qt::Unchecked);
    //Привязка действий
    connect(ui->editDslamBoardListTableView, &QTableView::customContextMenuRequested,
            this, &EditDslamBoardTablePageWidget::editBoardViewRequestContextMenu);
    connect(ui->autoUpdateBoardListCheckBox, &QCheckBox::toggled,
            this, &EditDslamBoardTablePageWidget::autoUpdateBoardListStateChanged);
    connect(ui->autoNumeringPairCheckBox, &QCheckBox::toggled,
            this, &EditDslamBoardTablePageWidget::autoNumeringPairsStateChanged);
}

EditDslamBoardTablePageWidget::~EditDslamBoardTablePageWidget()
{
    delete ui;
}

void EditDslamBoardTablePageWidget::editBoard()
{
    ui->editDslamBoardListTableView->setFocus();
    ui->editDslamBoardListTableView->edit(ui->editDslamBoardListTableView->currentIndex());
    mDeviceTableModel->setModified(true);
}

void EditDslamBoardTablePageWidget::removeBoard()
{
    QModelIndex index = ui->editDslamBoardListTableView->currentIndex();

    if (!index.isValid())
        return;

    BoardTableModel *model = mDeviceInfo.objectCast<Dslam>()->boardTableModel();
    QString num = model->data(model->index(index.row(), 0)).toString();
    QString type = model->data(model->index(index.row(), 1)).toString();

    if (!BasicDialogs::okToDelete(this, QString::fromUtf8("Удаление доски"),
                                  QString::fromUtf8("Вы действительно хотите удалить доску №%1(%2)?").arg(num, type)))
        return;

    model->removeRow(index.row(), QModelIndex());
    mDeviceTableModel->setModified(true);
}

void EditDslamBoardTablePageWidget::getBoardList()
{
    BoardTableModel *model = mDeviceInfo.objectCast<Dslam>()->boardTableModel();

    if (!model->getBoardListFromDevice())
        BasicDialogs::error(this, BasicDialogStrings::Error, model->error());

    mDeviceTableModel->setModified(true);
}

void EditDslamBoardTablePageWidget::renumeringBoardPairs()
{
    BoardTableModel *model = mDeviceInfo.objectCast<Dslam>()->boardTableModel();

    model->renumeringPairList();
    mDeviceTableModel->setModified(true);
}

void EditDslamBoardTablePageWidget::autoUpdateBoardListStateChanged(bool state)
{
    mDeviceInfo.objectCast<Dslam>()->setAutoFill(state ? 1 : 0);
    mDeviceTableModel->setModified(true);
    ui->autoNumeringPairCheckBox->setEnabled(state);
}

void EditDslamBoardTablePageWidget::autoNumeringPairsStateChanged(bool state)
{
    mDeviceInfo.objectCast<Dslam>()->setAutoNumeringBoard(state ? 1 : 0);
    mDeviceTableModel->setModified(true);
}

void EditDslamBoardTablePageWidget::editBoardViewRequestContextMenu(QPoint point)
{
    bool isEnabled = ui->autoUpdateBoardListCheckBox->checkState() == Qt::Checked;

    ui->editBoardAction->setEnabled(!isEnabled);
    ui->removeBoardAction->setEnabled(!isEnabled);
    ui->getBoardListAction->setEnabled(isEnabled);

    QMenu contextMenu(this);
    contextMenu.addAction(ui->editBoardAction);
    contextMenu.addAction(ui->removeBoardAction);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->getBoardListAction);
    contextMenu.addAction(ui->renumeringBoardPairsAction);

    contextMenu.exec(ui->editDslamBoardListTableView->mapToGlobal(point));
}


