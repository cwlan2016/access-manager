#include "editdslamboardlistpagewidget.h"
#include "ui_editdslamboardlistpagewidget.h"

#include <QtWidgets/QMenu>
#ifdef _MSC_VER
#include "../basicdialogs.h"
#include "../constant.h"
#include "../Info/dslaminfo.h"
#include "../Models/boardlistdelegate.h"
#else
#include "basicdialogs.h"
#include "constant.h"
#include "Info/dslaminfo.h"
#include "Models/boardlistdelegate.h"
#endif

EditDslamBoardListPageWidget::EditDslamBoardListPageWidget(DeviceInfo::Ptr deviceInfo, DeviceListModel *deviceListModel, QWidget *parent) :
    PageWidget(deviceInfo, parent),
    ui(new Ui::EditDslamBoardListPageWidget),
    mDeviceListModel(deviceListModel)
{
    ui->setupUi(this);

    connect(ui->editBoardAction, SIGNAL(triggered()), SLOT(editBoard()));
    connect(ui->removeBoardAction, SIGNAL(triggered()), SLOT(removeBoard()));
    connect(ui->getBoardListAction, SIGNAL(triggered()), SLOT(getBoardList()));
    connect(ui->renumeringBoardPairsAction, SIGNAL(triggered()), SLOT(renumeringBoardPairs()));

    BoardListModel *boardListModel = std::static_pointer_cast<DslamInfo>(mDeviceInfo)->boardListModel();

    ui->editDslamBoardListTableView->setModel(boardListModel);

    BoardListDelegate *boardListDelegate = new BoardListDelegate(mDeviceInfo->deviceModel(), this);
    boardListDelegate->setIndexTypeBoard(1);
    boardListDelegate->setIndexFirstPair(2);

    ui->editDslamBoardListTableView->setItemDelegate(boardListDelegate);

    ui->autoUpdateBoardListCheckBox->setCheckState(boardListModel->autoFill() == 1 ? Qt::Checked : Qt::Unchecked);
    ui->autoNumeringPairCheckBox->setEnabled(boardListModel->autoFill());
    ui->autoNumeringPairCheckBox->setCheckState(boardListModel->autoNumeringBoard() == 1 ? Qt::Checked : Qt::Unchecked);
    //Привязка действий
    connect(ui->editDslamBoardListTableView, SIGNAL(customContextMenuRequested(QPoint)), SLOT(editBoardViewRequestContextMenu(QPoint)));
    connect(ui->autoUpdateBoardListCheckBox, SIGNAL(toggled(bool)), SLOT(autoUpdateBoardListStateChanged(bool)));
    connect(ui->autoNumeringPairCheckBox, SIGNAL(toggled(bool)), SLOT(autoNumeringPairsStateChanged(bool)));
}

EditDslamBoardListPageWidget::~EditDslamBoardListPageWidget()
{
    delete ui;
}

void EditDslamBoardListPageWidget::editBoardViewRequestContextMenu(QPoint point)
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

void EditDslamBoardListPageWidget::renumeringBoardPairs()
{
    BoardListModel *model = std::static_pointer_cast<DslamInfo>(mDeviceInfo)->boardListModel();

    model->renumeringPairList();
    mDeviceListModel->setModified(true);
}

void EditDslamBoardListPageWidget::getBoardList()
{
    BoardListModel *model = std::static_pointer_cast<DslamInfo>(mDeviceInfo)->boardListModel();

    if (!model->getBoardListFromDevice())
        BasicDialogs::error(this, BasicDialogTitle::Error, model->error());

    mDeviceListModel->setModified(true);
}

void EditDslamBoardListPageWidget::editBoard()
{
    ui->editDslamBoardListTableView->setFocus();
    ui->editDslamBoardListTableView->edit(ui->editDslamBoardListTableView->currentIndex());
    mDeviceListModel->setModified(true);
}

void EditDslamBoardListPageWidget::removeBoard()
{
    QModelIndex index = ui->editDslamBoardListTableView->currentIndex();

    if (!index.isValid())
        return;

    BoardListModel *model = std::static_pointer_cast<DslamInfo>(mDeviceInfo)->boardListModel();
    QString num = model->data(model->index(index.row(), 0)).toString();
    QString type = model->data(model->index(index.row(), 1)).toString();

    if (!BasicDialogs::okToDelete(this, QString::fromUtf8("Удаление доски"),
                                  QString::fromUtf8("Вы действительно хотите удалить доску №%1(%2)?").arg(num, type)))
        return;

    model->removeRow(index.row(), QModelIndex());
    mDeviceListModel->setModified(true);
}

void EditDslamBoardListPageWidget::autoUpdateBoardListStateChanged(bool state)
{
    BoardListModel *model = std::static_pointer_cast<DslamInfo>(mDeviceInfo)->boardListModel();

    model->setAutoFill(state ? 1 : 0);
    mDeviceListModel->setModified(true);
    ui->autoNumeringPairCheckBox->setEnabled(state);
}

void EditDslamBoardListPageWidget::autoNumeringPairsStateChanged(bool state)
{
    BoardListModel *model = std::static_pointer_cast<DslamInfo>(mDeviceInfo)->boardListModel();

    model->setAutoNumeringBoard(state ? 1 : 0);
    mDeviceListModel->setModified(true);
}
