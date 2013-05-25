#include "dslampagewidget.h"
#include "ui_dslampagewidget.h"

#include <basicdialogs.h>
#include <constant.h>
#include <converters.h>
#include <devices/dslam.h>
#include <configs/dslamprofileconfig.h>
#include <models/boardtablemodel.h>
#include <models/dslamporttablemodel.h>
#include <models/dslprofiletablemodel.h>

DslamPageWidget::DslamPageWidget(Device::Ptr deviceInfo, QWidget *parent) :
    PageWidget(deviceInfo, parent),
    ui(new Ui::DslamPageWidget)
{
    ui->setupUi(this);

    ui->boardNameLabel->setVisible(false);
    ui->backToBoardListButton->setVisible(false);
    ui->selectProfileGroupBox->setVisible(false);
    ui->selectProfileGroupBox->setChecked(false);

    connect(ui->dslamTreeView, &QTreeView::customContextMenuRequested,
            this, &DslamPageWidget::viewRequestContextMenu);
    connect(ui->dslamTreeView, &QTreeView::doubleClicked,
            this, &DslamPageWidget::showPortListModel);
    connect(ui->dslamTreeView, &QTreeView::expanded,
            this, &DslamPageWidget::portListExpandedNode);
    connect(ui->selectProfileGroupBox, &QGroupBox::toggled,
            ui->selectProfileGroupBox, &QGroupBox::setVisible);
    connect(ui->backToBoardListButton, &QPushButton::pressed,
            this, &DslamPageWidget::backToBoardListModel);
    connect(ui->applyProfileButton, &QPushButton::pressed,
            this, &DslamPageWidget::applyDslProfile);

    connect(ui->showBoardAction, &QAction::triggered,
            this, &DslamPageWidget::showPortListModel);
    connect(ui->refreshPortInfoAction, &QAction::triggered,
            this, &DslamPageWidget::refreshPortInfo);
    connect(ui->upPortAction, &QAction::triggered,
            this, &DslamPageWidget::upDslPort);
    connect(ui->downPortAction, &QAction::triggered,
            this, &DslamPageWidget::downDslPort);
    connect(ui->showSelectProfileGBAction, &QAction::triggered,
            this, &DslamPageWidget::showSelectProfileGBox);
    connect(ui->collapseAllNodeAction, &QAction::triggered,
            ui->dslamTreeView, &QTreeView::collapseAll);
    connect(ui->refreshAllPortInfoAction, &QAction::triggered,
            this, &DslamPageWidget::refreshAllPortInfo);

    if ((mDeviceInfo->deviceModel() == DeviceModel::MA5600)
            || (mDeviceInfo->deviceModel() == DeviceModel::MA5300)) {
        ui->dslamTreeView->setModel(mDeviceInfo.objectCast<Dslam>()->boardTableModel());
    } else {
        DslamPortTableModel *portListModel = new DslamPortTableModel(mDeviceInfo.objectCast<Dslam>(), this);

        portListModel->setBoardType(BoardType::AnnexA);
        portListModel->setFirstPair(1);
        portListModel->setBoardIndex(1);
        portListModel->createList();

        if (!portListModel->load()) {
            BasicDialogs::error(this, BasicDialogStrings::Error, portListModel->error());
        }

        ui->dslamTreeView->setModel(portListModel);

        fillSelectProfileComboBox();
    }
}

DslamPageWidget::~DslamPageWidget()
{
    delete ui;
}

void DslamPageWidget::upDslPort()
{
    QModelIndex currentPort = currentDslamXdslPort();

    if (qstrcmp(ui->dslamTreeView->model()->metaObject()->className(), "DslamPortTableModel") != 0)
        return;

    DslamPortTableModel *portListModel = qobject_cast<DslamPortTableModel *>(ui->dslamTreeView->model());

    if (portListModel->changePortState(currentPort.row(), DslPortState::Up)) {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Порт активирован."));
        portListModel->updatePortInfo(currentPort);
    } else {
        BasicDialogs::warning(this, BasicDialogStrings::Warning, QString::fromUtf8("Порт активировать не удалось."));
    }
}

void DslamPageWidget::downDslPort()
{
    QModelIndex currentPort = currentDslamXdslPort();

    if (qstrcmp(ui->dslamTreeView->model()->metaObject()->className(), "DslamPortTableModel") != 0)
        return;

    DslamPortTableModel *portListModel = qobject_cast<DslamPortTableModel *>(ui->dslamTreeView->model());

    if (portListModel->changePortState(currentPort.row(), DslPortState::Down)) {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Порт деактивирован."));
        portListModel->updatePortInfo(currentPort);
    } else {
        BasicDialogs::warning(this, BasicDialogStrings::Warning, QString::fromUtf8("Порт деактивировать не удалось."));
    }
}

void DslamPageWidget::showPortListModel()
{
    if (!ui->boardNameLabel->isHidden())
        return;

    if (!ui->dslamTreeView->currentIndex().isValid())
        return;

    if (qstrcmp(ui->dslamTreeView->model()->metaObject()->className(), "BoardTableModel") != 0)
        return;

    BoardTableModel *boardListModel = static_cast<BoardTableModel *>(ui->dslamTreeView->model());

    QModelIndex indexTypeBoard = boardListModel->index(ui->dslamTreeView->currentIndex().row(), 1);
    BoardType::Enum typeBoard = (BoardType::Enum)boardListModel->data(indexTypeBoard, Qt::UserRole).toInt();

    if (typeBoard == BoardType::Other) {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Выберите существующую доску."));
        return;
    }

    if (typeBoard == BoardType::Shdsl) {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Информация доступна только по ADSL доскам."));
        return;
    }

    DslamPortTableModel *portListModel = new DslamPortTableModel(mDeviceInfo.objectCast<Dslam>(), this);

    QModelIndex indexPairRange = boardListModel->index(ui->dslamTreeView->currentIndex().row(), 2);
    QString pair = boardListModel->data(indexPairRange).toString().split("-")[0];

    portListModel->setBoardType(typeBoard);
    portListModel->setFirstPair(pair.toInt());
    portListModel->setBoardIndex(indexPairRange.row());
    portListModel->createList();

    if (!portListModel->load())
        BasicDialogs::error(this, BasicDialogStrings::Error, portListModel->error());

    ui->boardNameLabel->setText(QString::fromUtf8("Доска %1 [%2]")
                                .arg(indexPairRange.row())
                                .arg(BoardType::toString(typeBoard)));
    ui->boardNameLabel->setVisible(true);
    ui->backToBoardListButton->setVisible(true);
    ui->dslamTreeView->setModel(portListModel);
    ui->dslamTreeView->setColumnWidth(0, 220);
    ui->dslamTreeView->setColumnWidth(1, 120);
    ui->dslamTreeView->setColumnWidth(2, 100);
    ui->dslamTreeView->setColumnWidth(3, 120);

    fillSelectProfileComboBox();
}

void DslamPageWidget::backToBoardListModel()
{
    ui->boardNameLabel->setVisible(false);
    ui->backToBoardListButton->setVisible(false);
    ui->selectProfileGroupBox->setChecked(false);

    DslamPortTableModel *portTableModel = qobject_cast<DslamPortTableModel *>(ui->dslamTreeView->model());
    BoardTableModel *boardTableModel = mDeviceInfo.objectCast<Dslam>()->boardTableModel();

    ui->dslamTreeView->setModel(boardTableModel);

    delete portTableModel;
}

void DslamPageWidget::showSelectProfileGBox()
{
    ui->selectProfileGroupBox->setChecked(true);
}

void DslamPageWidget::fillSelectProfileComboBox()
{
    DslamPortTableModel *model = qobject_cast<DslamPortTableModel *>(ui->dslamTreeView->model());

    if (!model)
        return;

    DslProfileTableModel *dslProfileModel = 0;
    if ((model->boardType() == BoardType::AnnexA)
            || (model->boardType() == BoardType::AnnexB)) {
        dslProfileModel = DslamProfileConfig::adsl(mDeviceInfo->deviceModel());
    } else if (model->boardType() == BoardType::Shdsl) {
        dslProfileModel = DslamProfileConfig::shdsl(mDeviceInfo->deviceModel());
    }

    ui->profileListComboBox->setModel(dslProfileModel);
}

void DslamPageWidget::applyDslProfile()
{
    if (!ui->dslamTreeView->currentIndex().isValid())
        return;

    if (ui->dslamTreeView->currentIndex().internalId() != invalidParentIndex) {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Выберите порт для изменения профиля."));
        return;
    }

    DslamPortTableModel *portListModel = qobject_cast<DslamPortTableModel *>(ui->dslamTreeView->model());
    DslProfileTableModel *dslProfileModel = qobject_cast<DslProfileTableModel *>(ui->profileListComboBox->model());

    if (!dslProfileModel)
        return;

    QString profileName = dslProfileModel->dslamProfileName(ui->profileListComboBox->currentIndex());

    bool result = portListModel->changePortProfile(ui->dslamTreeView->currentIndex(), profileName);

    if (result) {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Профиль на порту изменен."));
        portListModel->updatePortInfo(ui->dslamTreeView->currentIndex());
    } else {
        BasicDialogs::warning(this, BasicDialogStrings::Warning, QString::fromUtf8("Профиль на порту изменить не удалось!"));
    }
}

void DslamPageWidget::refreshPortInfo()
{
    if (!ui->dslamTreeView->currentIndex().isValid())
        return;

    if (ui->dslamTreeView->currentIndex().internalId() != invalidParentIndex) {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Выберите порт для обновления информации."));
        return;
    }

    portListExpandedNode(ui->dslamTreeView->currentIndex());
}

void DslamPageWidget::refreshAllPortInfo()
{
    DslamPortTableModel *portListModel = qobject_cast<DslamPortTableModel *>(ui->dslamTreeView->model());

    if (!portListModel->load()) {
        BasicDialogs::error(this, BasicDialogStrings::Error, portListModel->error());
    } else {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Информация по портам обновлена."));
    }
}

void DslamPageWidget::viewRequestContextMenu(QPoint point)
{
    QMenu contextMenu(this);

    if (qstrcmp(ui->dslamTreeView->model()->metaObject()->className(), "BoardTableModel") == 0) {
        contextMenu.addAction(ui->showBoardAction);
    } else {
        contextMenu.addAction(ui->refreshPortInfoAction);
        contextMenu.addSeparator();
        contextMenu.addAction(ui->upPortAction);
        contextMenu.addAction(ui->downPortAction);
        contextMenu.addAction(ui->showSelectProfileGBAction);
        contextMenu.addSeparator();
        contextMenu.addAction(ui->collapseAllNodeAction);
        contextMenu.addAction(ui->refreshAllPortInfoAction);
    }

    contextMenu.exec(ui->dslamTreeView->mapToGlobal(point));
}

void DslamPageWidget::portListExpandedNode(QModelIndex index)
{
    if (!index.isValid())
        return;

    if (index.internalId() != invalidParentIndex)
        return;

    DslamPortTableModel *portListModel = qobject_cast<DslamPortTableModel *>(ui->dslamTreeView->model());

    if (!portListModel->updatePortInfo(index))
        BasicDialogs::error(this, BasicDialogStrings::Error, portListModel->error());
}

QModelIndex DslamPageWidget::currentDslamXdslPort()
{
    if (!ui->dslamTreeView->currentIndex().isValid())
        return QModelIndex();

    if (ui->dslamTreeView->currentIndex().internalId() == invalidParentIndex) {
        return ui->dslamTreeView->currentIndex();
    } else {
        return ui->dslamTreeView->currentIndex().parent();
    }
}
