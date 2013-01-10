#include "dslampagewidget.h"
#include "ui_dslampagewidget.h"

#include <QtWidgets/QMenu>
#ifdef _MSC_VER
#include "../basicdialogs.h"
#include "../constant.h"
#include "../converters.h"
#include "../Info/dslaminfo.h"
#include "../Models/boardlistmodel.h"
#include "../Models/dslamportlistmodel.h"
#else
#include "basicdialogs.h"
#include "constant.h"
#include "converters.h"
#include "Info/dslaminfo.h"
#include "Models/boardlistmodel.h"
#include "Models/dslamportlistmodel.h"
#endif

DslamPageWidget::DslamPageWidget(DeviceInfo::Ptr deviceInfo, QWidget *parent) :
    PageWidget(deviceInfo, parent),
    ui(new Ui::DslamPageWidget)
{
    ui->setupUi(this);

    ui->boardNameLabel->setVisible(false);
    ui->backToBoardListButton->setVisible(false);
    ui->selectProfileGroupBox->setVisible(false);

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

    fillSelectProfileComboBox();

    if ((mDeviceInfo->deviceModel() == DeviceModel::MA5600)
            || (mDeviceInfo->deviceModel() == DeviceModel::MA5300)) {
        ui->dslamTreeView->setModel(std::static_pointer_cast<DslamInfo>(mDeviceInfo)->boardListModel());
    } else {
        DslamPortListModel *portListModel = new DslamPortListModel(mDeviceInfo->deviceModel(), mDeviceInfo->ip(), this);

        portListModel->setBoardType(BoardType::AnnexA);
        portListModel->setFirstPair(1);
        portListModel->setBoardNumber(1);
        portListModel->createList();

        if (!portListModel->load()) {
            BasicDialogs::error(this, BasicDialogTitle::Error, portListModel->error());
        }

        ui->dslamTreeView->setModel(portListModel);
    }
}

DslamPageWidget::~DslamPageWidget()
{

    if (typeid(DslamPortListModel) == typeid(*ui->dslamTreeView->model())) {
        DslamPortListModel *model = static_cast<DslamPortListModel *>(ui->dslamTreeView->model());
        delete model;
    }

    delete ui;

}

void DslamPageWidget::fillSelectProfileComboBox()
{
    if ((mDeviceInfo->deviceModel() == DeviceModel::MA5600) ||
            (mDeviceInfo->deviceModel() == DeviceModel::MA5300)) {
        ui->profileListComboBox->addItem(AdslProfileDisplayName::AdslFast);
        ui->profileListComboBox->addItem(AdslProfileDisplayName::AdslInterleave);
        ui->profileListComboBox->addItem(AdslProfileDisplayName::Adsl2Interleave);

        if (mDeviceInfo->deviceModel() == DeviceModel::MA5600) {
            ui->profileListComboBox->addItem(AdslProfileDisplayName::AdslFast8Mb);
            ui->profileListComboBox->addItem(AdslProfileDisplayName::Adsl2Fast10Mb);
            ui->profileListComboBox->addItem(AdslProfileDisplayName::Adsl2Fast14Mb);
            ui->profileListComboBox->addItem(AdslProfileDisplayName::Adsl2Fast18Mb);
        }
    } else if ((mDeviceInfo->deviceModel() == DeviceModel::MXA64) ||
               (mDeviceInfo->deviceModel() == DeviceModel::MXA32)) {
        ui->profileListComboBox->addItem(AdslProfileDisplayName::AdslInterleave);
        ui->profileListComboBox->addItem(AdslProfileDisplayName::AdslGLite);
        ui->profileListComboBox->addItem(AdslProfileDisplayName::Adsl2Interleave);
    }
}

QModelIndex DslamPageWidget::currentDslamXdslPort()
{
    if (!ui->dslamTreeView->currentIndex().isValid())
        return QModelIndex();

    if (ui->dslamTreeView->currentIndex().internalId() == -1) {
        return ui->dslamTreeView->currentIndex();
    } else {
        return ui->dslamTreeView->currentIndex().parent();
    }
}

void DslamPageWidget::upDslPort()
{
    QModelIndex currentPort = currentDslamXdslPort();

    if (typeid(DslamPortListModel) != typeid(*ui->dslamTreeView->model()))
        return;

    DslamPortListModel *portListModel = static_cast<DslamPortListModel *>(ui->dslamTreeView->model());

    if (portListModel->changePortState(currentPort.row(), "1")) {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Порт активирован."));
        portListModel->updatePortInfo(currentPort);
    } else {
        BasicDialogs::warning(this, BasicDialogTitle::Warning, QString::fromUtf8("Порт активировать не удалось."));
    }
}

void DslamPageWidget::downDslPort()
{
    QModelIndex currentPort = currentDslamXdslPort();

    if (typeid(DslamPortListModel) != typeid(*ui->dslamTreeView->model()))
        return;

    DslamPortListModel *portListModel = static_cast<DslamPortListModel *>(ui->dslamTreeView->model());

    if (portListModel->changePortState(currentPort.row(), "2")) {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Порт деактивирован."));
        portListModel->updatePortInfo(currentPort);
    } else {
        BasicDialogs::warning(this, BasicDialogTitle::Warning, QString::fromUtf8("Порт деактивировать не удалось."));
    }
}

void DslamPageWidget::viewRequestContextMenu(QPoint point)
{
    QMenu contextMenu(this);

    if (typeid(BoardListModel) == typeid(*ui->dslamTreeView->model())) {
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

void DslamPageWidget::showPortListModel()
{
    if (!ui->boardNameLabel->isHidden())
        return;

    if (!ui->dslamTreeView->currentIndex().isValid())
        return;

    if (typeid(BoardListModel) != typeid(*ui->dslamTreeView->model()))
        return;

    BoardListModel *boardListModel = static_cast<BoardListModel *>(ui->dslamTreeView->model());

    QModelIndex indexTypeBoard = boardListModel->index(ui->dslamTreeView->currentIndex().row(), 1);
    BoardType::Enum typeBoard = (BoardType::Enum)boardListModel->data(indexTypeBoard, Qt::UserRole).toInt();

    if (typeBoard == BoardType::Other) {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Выберите существующую доску."));
        return;
    }

    if (typeBoard == BoardType::Shdsl) {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Информация доступна только по ADSL доскам."));
        return;
    }

    DslamPortListModel *portListModel = new DslamPortListModel(mDeviceInfo->deviceModel(), mDeviceInfo->ip());//new DslamPortListModel(boardListModel->parentDevice());

    QModelIndex indexPairRange = boardListModel->index(ui->dslamTreeView->currentIndex().row(), 2);
    QString pair = boardListModel->data(indexPairRange).toString().split("-")[0];

    portListModel->setBoardType(typeBoard);
    portListModel->setFirstPair(pair.toInt());
    portListModel->setBoardNumber(indexPairRange.row());
    portListModel->createList();

    if (!portListModel->load()) {
        BasicDialogs::error(this, BasicDialogTitle::Error, portListModel->error());
    }

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
}

void DslamPageWidget::backToBoardListModel()
{
    ui->boardNameLabel->setVisible(false);
    ui->backToBoardListButton->setVisible(false);
    ui->selectProfileGroupBox->setChecked(false);

    DslamPortListModel *portListModel = static_cast<DslamPortListModel *>(ui->dslamTreeView->model());
    BoardListModel *boardListModel = std::static_pointer_cast<DslamInfo>(mDeviceInfo)->boardListModel();

    ui->dslamTreeView->setModel(boardListModel);

    delete portListModel;
}

void DslamPageWidget::showSelectProfileGBox()
{
    DslamPortListModel *portListModel = static_cast<DslamPortListModel *>(ui->dslamTreeView->model());

    DeviceModel::Enum deviceModel = mDeviceInfo->deviceModel();

    if ((portListModel->boardType() == BoardType::AnnexA)
            && (deviceModel == DeviceModel::MA5600)) {
        if (ui->profileListComboBox->count() <= 7)
            ui->profileListComboBox->addItem(AdslProfileDisplayName::AdslGLite);
    } else {
        if (ui->profileListComboBox->count() == 8)
            ui->profileListComboBox->removeItem(7);
    }

    ui->selectProfileGroupBox->setChecked(true);
}

void DslamPageWidget::applyDslProfile()
{
    if (!ui->dslamTreeView->currentIndex().isValid())
        return;

    if (ui->dslamTreeView->currentIndex().internalId() != -1) {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Выберите порт для изменения профиля."));
        return;
    }

    DslamPortListModel *portListModel = static_cast<DslamPortListModel *>(ui->dslamTreeView->model());
    QString profileName = displayNameProfileToDslamName(mDeviceInfo->deviceModel(), ui->profileListComboBox->currentText());

    bool result = portListModel->changePortProfile(ui->dslamTreeView->currentIndex(), profileName);

    if (result) {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Профиль на порту изменен."));
        portListModel->updatePortInfo(ui->dslamTreeView->currentIndex());
    } else {
        BasicDialogs::warning(this, BasicDialogTitle::Warning, QString::fromUtf8("Профиль на порту изменить не удалось!"));
    }
}

void DslamPageWidget::refreshPortInfo()
{
    if (!ui->dslamTreeView->currentIndex().isValid())
        return;

    if (ui->dslamTreeView->currentIndex().internalId() != -1) {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Выберите порт для обновления информации."));
        return;
    }

    portListExpandedNode(ui->dslamTreeView->currentIndex());
}

void DslamPageWidget::refreshAllPortInfo()
{
    DslamPortListModel *portListModel = static_cast<DslamPortListModel *>(ui->dslamTreeView->model());

    if (!portListModel->load()) {
        BasicDialogs::error(this, BasicDialogTitle::Error, portListModel->error());
    } else {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Информация по портам обновлена."));
    }
}

void DslamPageWidget::portListExpandedNode(QModelIndex index)
{
    if (!index.isValid())
        return;

    if (index.internalId() != -1)
        return;

    DslamPortListModel *portListModel = static_cast<DslamPortListModel *>(ui->dslamTreeView->model());

    if (!portListModel->updatePortInfo(index))
        BasicDialogs::error(this, BasicDialogTitle::Error, portListModel->error());
}
