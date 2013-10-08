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
    DevicePageWidget(deviceInfo, parent),
    ui(new Ui::DslamPageWidget)
{
    ui->setupUi(this);     

    ui->messageWidget->hide();
    ui->messageWidget->setCloseButtonVisible(true);

    ui->updatePortFrame->hide();
}

DslamPageWidget::~DslamPageWidget()
{
    delete ui;
}

void DslamPageWidget::init()
{
    initActions();
    initComponents();
    initView();
    setupMenu();
}

void DslamPageWidget::showMessage(const QString &msg, ImprovedMessageWidget::MessageType messageType, const QString &detailedText)
{
    if (msg.isEmpty())
        return;

    ui->messageWidget->setText(msg, detailedText);
    ui->messageWidget->setMessageType(messageType);

    ui->messageWidget->setWordWrap(false);
    const int unwrappedWidth = ui->messageWidget->sizeHint().width();
    ui->messageWidget->setWordWrap(unwrappedWidth > size().width());

    ui->messageWidget->animatedShow();
}

void DslamPageWidget::initActions()
{
    connect(ui->showBoardAction, &QAction::triggered,
            this, &DslamPageWidget::showPortListModel);
    connect(ui->refreshPortInfoAction, &QAction::triggered,
            this, &DslamPageWidget::refreshPortInfo);
    connect(ui->upPortAction, &QAction::triggered,
            this, &DslamPageWidget::upDslPort);
    connect(ui->downPortAction, &QAction::triggered,
            this, &DslamPageWidget::downDslPort);
    connect(ui->editPortDescAction, &QAction::triggered,
            this, &DslamPageWidget::editDescPort);
    connect(ui->showProfileFrameAction, &QAction::triggered,
            ui->profileFrame, &QFrame::show);
    connect(ui->collapseAllNodeAction, &QAction::triggered,
            ui->dslamTreeView, &QTreeView::collapseAll);
    connect(ui->refreshAllPortInfoAction, &QAction::triggered,
            this, &DslamPageWidget::refreshAllPortInfo);
    connect(ui->editBoardAction, &QAction::triggered,
            this, &DslamPageWidget::editBoard);
    connect(ui->removeBoardAction, &QAction::triggered,
            this, &DslamPageWidget::removeBoard);
    connect(ui->getBoardListAction, &QAction::triggered,
            this, &DslamPageWidget::getBoardList);
    connect(ui->renumeringPairAction, &QAction::triggered,
            this, &DslamPageWidget::renumeringPairs);

    ui->upPortButton->setDefaultAction(ui->upPortAction);
    ui->downPortButton->setDefaultAction(ui->downPortAction);
    ui->showProfileFrameButton->setDefaultAction(ui->showProfileFrameAction);
    ui->editPortDescButton->setDefaultAction(ui->editPortDescAction);
    ui->collapseAllNodeButton->setDefaultAction(ui->collapseAllNodeAction);
    ui->refreshAllPortInfoButton->setDefaultAction(ui->refreshAllPortInfoAction);
    ui->editBoardButton->setDefaultAction(ui->editBoardAction);
    ui->removeBoardButton->setDefaultAction(ui->removeBoardAction);
}

void DslamPageWidget::initComponents()
{
    mEditMode = false;

    ui->backToBoardListButton->setVisible(false);
    ui->boardNameLabel->setVisible(false);
    ui->profileFrame->setVisible(false);
    ui->editToolButtonPanel->setVisible(false);
    ui->finishEditButton->setVisible(false);

    connect(ui->closeProfileFrameButton, &QToolButton::pressed,
            ui->profileFrame, &QFrame::hide);
    connect(ui->backToBoardListButton, &QToolButton::pressed,
            this, &DslamPageWidget::backToBoardListModel);
    connect(ui->applyProfileButton, &QToolButton::pressed,
            this, &DslamPageWidget::applyDslProfile);

    Dslam::Ptr dslamInfo = mDevice.objectCast<Dslam>();

    ui->autoUpdateBoardListCheckBox->setCheckState(dslamInfo->autoFill() == 1 ? Qt::Checked : Qt::Unchecked);
    ui->autoNumeringPairCheckBox->setEnabled(dslamInfo->autoFill());
    ui->autoNumeringPairCheckBox->setCheckState(dslamInfo->autoNumeringBoard() == 1 ? Qt::Checked : Qt::Unchecked);
    autoNumeringPairsStateChanged(dslamInfo->autoNumeringBoard());
    autoUpdateBoardListStateChanged(dslamInfo->autoFill());

    mBoardTableDelegate = new BoardTableDelegate(mDevice.objectCast<Dslam>(), this);

    mPortTableDelegate = new DslamPortTableDelegate(this);
    mPortTableDelegate->setDescriptionPortLength(mDevice->maxLengthPortDescription());

    connect(ui->autoUpdateBoardListCheckBox, &QCheckBox::toggled,
            this, &DslamPageWidget::autoUpdateBoardListStateChanged);
    connect(ui->autoNumeringPairCheckBox, &QCheckBox::toggled,
            this, &DslamPageWidget::autoNumeringPairsStateChanged);
    connect(ui->beginEditButton, &QToolButton::pressed,
            this, &DslamPageWidget::beginEditBoardList);
    connect(ui->finishEditButton, &QPushButton::pressed,
            this, &DslamPageWidget::finishEditBoardList);
    connect(this, &DslamPageWidget::modeChanged,
            this, &DslamPageWidget::pageModeChanged);
}

void DslamPageWidget::initView()
{
    if ((mDevice->deviceModel() == DeviceModel::MA5600)
            || (mDevice->deviceModel() == DeviceModel::MA5300)) {
        ui->dslamTreeView->setModel(mDevice.objectCast<Dslam>()->boardTableModel());
        ui->operationToolButtonPanel->setVisible(false);
    } else {
        ui->topButtonPanel->setVisible(false);

        DslamPortTableModel *portListModel = new DslamPortTableModel(mDevice.objectCast<Dslam>(),
                                                                     ui->messageWidget, this);
        connect(portListModel, &DslamPortTableModel::updateFinished,
                this, &DslamPageWidget::updatePortTableFinished);

        portListModel->setBoardType(BoardType::AnnexA);
        portListModel->setFirstPair(1);
        portListModel->setBoardIndex(1);
        portListModel->createList();

        ui->updatePortFrame->show();
        portListModel->update();

        ui->dslamTreeView->setModel(portListModel);
        ui->dslamTreeView->setItemDelegate(mPortTableDelegate);
        ui->dslamTreeView->setEditTriggers(QAbstractItemView::EditKeyPressed);

        fillSelectProfileComboBox();
        setupMenu();
    }

    ui->dslamTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    connect(ui->dslamTreeView, &QTreeView::customContextMenuRequested,
            this, &DslamPageWidget::viewRequestContextMenu);
    connect(ui->dslamTreeView, &QTreeView::doubleClicked,
            this, &DslamPageWidget::showPortListModel);
    connect(ui->dslamTreeView, &QTreeView::expanded,
            this, &DslamPageWidget::portListExpandedNode);
}

void DslamPageWidget::upDslPort()
{
    QModelIndex currentPort = currentDslamXdslPort();

    if (!currentPort.isValid())
        return;

    if (qstrcmp(ui->dslamTreeView->model()->metaObject()->className(), "DslamPortTableModel") != 0)
        return;

    DslamPortTableModel *portListModel = qobject_cast<DslamPortTableModel *>(ui->dslamTreeView->model());

    QModelIndex portIndex = portListModel->index(currentPort.row(), DslamPortTableModel::DescColumn, currentPort.parent());
    QString name = portListModel->data(portIndex).toString();

    if (portListModel->changePortState(currentPort.row(), DslPortState::Up)) {
        showMessage(trUtf8("Порт %1 активирован.").arg(name));
        bool result = portListModel->updatePortBasic(currentPort);

        if (!result) {
            showMessage(trUtf8("Не удалось обновить информацию по порту %1.").arg(name),
                        ImprovedMessageWidget::Error, portListModel->error());
        } else {
            ui->dslamTreeView->setExpanded(currentPort, false);
        }
    } else {
        showMessage(trUtf8("Порт %1 активировать не удалось.").arg(name),
                    ImprovedMessageWidget::Warning);
    }
}

void DslamPageWidget::downDslPort()
{
    QModelIndex currentPort = currentDslamXdslPort();

    if (!currentPort.isValid())
        return;

    if (qstrcmp(ui->dslamTreeView->model()->metaObject()->className(), "DslamPortTableModel") != 0)
        return;

    DslamPortTableModel *portListModel = qobject_cast<DslamPortTableModel *>(ui->dslamTreeView->model());

    QModelIndex portIndex = portListModel->index(currentPort.row(), DslamPortTableModel::DescColumn, currentPort.parent());
    QString name = portListModel->data(portIndex).toString();

    if (portListModel->changePortState(currentPort.row(), DslPortState::Down)) {
        showMessage(trUtf8("Порт %1 деактивирован.").arg(name));

        bool result = portListModel->updatePortBasic(currentPort);

        if (!result) {
            showMessage(trUtf8("Не удалось обновить информацию по порту %1.").arg(name),
                        ImprovedMessageWidget::Error, portListModel->error());
        } else {
            ui->dslamTreeView->setExpanded(currentPort, false);
        }
    } else {
        showMessage(trUtf8("Порт %1 деактивировать не удалось.").arg(name),
                    ImprovedMessageWidget::Warning);
    }
}

void DslamPageWidget::editDescPort()
{
    QModelIndex currentPort = currentDslamXdslPort();

    if (!currentPort.isValid())
        return;

    if (qstrcmp(ui->dslamTreeView->model()->metaObject()->className(), "DslamPortTableModel") != 0)
        return;

    QModelIndex descIndex = ui->dslamTreeView->model()->index(currentPort.row(),
                                                              DslamPortTableModel::DescColumn);
    ui->dslamTreeView->setFocus();
    ui->dslamTreeView->edit(descIndex);
}

void DslamPageWidget::showPortListModel()
{
    if (!ui->backToBoardListButton->isHidden())
        return;

    if (!ui->dslamTreeView->currentIndex().isValid())
        return;

    if (qstrcmp(ui->dslamTreeView->model()->metaObject()->className(), "BoardTableModel") != 0)
        return;

    BoardTableModel *boardListModel = static_cast<BoardTableModel *>(ui->dslamTreeView->model());

    QModelIndex indexTypeBoard = boardListModel->index(ui->dslamTreeView->currentIndex().row(), 1);
    BoardType::Enum typeBoard = (BoardType::Enum)boardListModel->data(indexTypeBoard, Qt::UserRole).toInt();

    if (typeBoard == BoardType::Other) {
        showMessage(trUtf8("Выберите существующую доску."));
        return;
    }

    DslamPortTableModel *portListModel = new DslamPortTableModel(mDevice.objectCast<Dslam>(),
                                                                 ui->messageWidget, this);
    connect(portListModel, &DslamPortTableModel::updateFinished,
            this, &DslamPageWidget::updatePortTableFinished);

    QModelIndex indexPairRange = boardListModel->index(ui->dslamTreeView->currentIndex().row(), 2);
    QString pair = boardListModel->data(indexPairRange).toString().split("-")[0];

    portListModel->setBoardType(typeBoard);
    portListModel->setFirstPair(pair.toInt());
    portListModel->setBoardIndex(indexPairRange.row());
    portListModel->createList();

    ui->updatePortFrame->show();
    portListModel->update();

    ui->boardNameLabel->setText(QString::fromUtf8("Доска %1 [%2]")
                                .arg(indexPairRange.row())
                                .arg(BoardType::toString(typeBoard)));
    ui->backToBoardListButton->setVisible(true);
    ui->boardNameLabel->setVisible(true);
    ui->beginEditButton->setVisible(false);
    ui->operationToolButtonPanel->setVisible(true);
    ui->dslamTreeView->setModel(portListModel);
    ui->dslamTreeView->setItemDelegate(mPortTableDelegate);
    ui->dslamTreeView->setEditTriggers(QAbstractItemView::EditKeyPressed);

    fillSelectProfileComboBox();
    setupMenu();
}

void DslamPageWidget::backToBoardListModel()
{
    ui->backToBoardListButton->hide();
    ui->boardNameLabel->hide();
    ui->beginEditButton->show();
    ui->operationToolButtonPanel->hide();
    ui->profileFrame->hide();
    ui->updatePortFrame->hide();

    DslamPortTableModel *portTableModel = qobject_cast<DslamPortTableModel *>(ui->dslamTreeView->model());
    BoardTableModel *boardTableModel = mDevice.objectCast<Dslam>()->boardTableModel();

    ui->dslamTreeView->setModel(boardTableModel);
    ui->dslamTreeView->setItemDelegate(new QItemDelegate(this));
    ui->dslamTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    delete portTableModel;

    setupMenu();
}

void DslamPageWidget::fillSelectProfileComboBox()
{
    DslamPortTableModel *model = qobject_cast<DslamPortTableModel *>(ui->dslamTreeView->model());

    if (!model)
        return;

    DslProfileTableModel *dslProfileModel = 0;
    if ((model->boardType() == BoardType::AnnexA)
            || (model->boardType() == BoardType::AnnexB)) {
        dslProfileModel = DslamProfileConfig::adsl(mDevice->deviceModel());
    } else if (model->boardType() == BoardType::Shdsl) {
        dslProfileModel = DslamProfileConfig::shdsl(mDevice->deviceModel());
    }

    ui->profileListComboBox->setModel(dslProfileModel);
}

void DslamPageWidget::applyDslProfile()
{
    QModelIndex currentPort = currentDslamXdslPort();

    if (!currentPort.isValid())
        return;

    if (ui->profileListComboBox->currentText().isEmpty()) {
        showMessage(trUtf8("Выберите профиль, который вы хотите применить."));
        return;
    }

    DslamPortTableModel *portListModel = qobject_cast<DslamPortTableModel *>(ui->dslamTreeView->model());
    DslProfileTableModel *dslProfileModel = qobject_cast<DslProfileTableModel *>(ui->profileListComboBox->model());

    if (!dslProfileModel)
        return;

    QString profileName = dslProfileModel->dslamProfileName(ui->profileListComboBox->currentIndex());

    bool result = portListModel->changePortProfile(currentPort, profileName);

    QModelIndex portIndex = portListModel->index(currentPort.row(), DslamPortTableModel::DescColumn, currentPort.parent());
    QString name = portListModel->data(portIndex).toString();

    if (result) {
        showMessage(trUtf8("Профиль на порту %1 изменен.").arg(name));
        bool result = portListModel->updatePortBasic(ui->dslamTreeView->currentIndex());

        if (!result) {
            showMessage(trUtf8("Не удалось обновить информацию по порту %1.").arg(name),
                        ImprovedMessageWidget::Error, portListModel->error());
        } else {
            ui->dslamTreeView->setExpanded(currentPort, false);
        }
    } else {
        showMessage(trUtf8("Профиль на порту %1 изменить не удалось!").arg(name),
                    ImprovedMessageWidget::Warning);
    }
}

void DslamPageWidget::refreshPortInfo()
{
    QModelIndex currentPort = currentDslamXdslPort();

    if (!currentPort.isValid())
        return;

    DslamPortTableModel *portListModel = qobject_cast<DslamPortTableModel *>(ui->dslamTreeView->model());

    QModelIndex portIndex = portListModel->index(currentPort.row(), DslamPortTableModel::DescColumn, currentPort.parent());
    QString name = portListModel->data(portIndex).toString();

    if (!portListModel->updatePortBasic(currentPort)) {
        showMessage(trUtf8("Не удалось обновить информацию по порту %1.").arg(name),
                    ImprovedMessageWidget::Error, portListModel->error());
    } else {
        showMessage(trUtf8("Информация по порту %1 успешно обновлена.").arg(name),
                    ImprovedMessageWidget::Warning);
        ui->dslamTreeView->setExpanded(currentPort, false);
    }

}

void DslamPageWidget::refreshAllPortInfo()
{
    DslamPortTableModel *portListModel = qobject_cast<DslamPortTableModel *>(ui->dslamTreeView->model());
    ui->updatePortFrame->show();
    portListModel->update();
}

void DslamPageWidget::autoUpdateBoardListStateChanged(bool state)
{
    mDevice.objectCast<Dslam>()->setAutoFill(state ? 1 : 0);
    ui->autoNumeringPairCheckBox->setEnabled(state);

    currentColumnChanged(ui->dslamTreeView->currentIndex(), QModelIndex());

    ui->getBoardListAction->setEnabled(state);
}

void DslamPageWidget::autoNumeringPairsStateChanged(bool state)
{
    mDevice.objectCast<Dslam>()->setAutoNumeringBoard(state ? 1 : 0);
    currentColumnChanged(ui->dslamTreeView->currentIndex(), QModelIndex());
}

void DslamPageWidget::editBoard()
{
    ui->dslamTreeView->setFocus();
    ui->dslamTreeView->edit(ui->dslamTreeView->currentIndex());
}

void DslamPageWidget::removeBoard()
{
    QModelIndex index = ui->dslamTreeView->currentIndex();

    if (!index.isValid())
        return;

    BoardTableModel *model = mDevice.objectCast<Dslam>()->boardTableModel();
    QString num = model->data(model->index(index.row(), 0)).toString();
    QString type = model->data(model->index(index.row(), 1)).toString();

    if (!BasicDialogs::okToDelete(this, QString::fromUtf8("Удаление доски"),
                                  QString::fromUtf8("Вы действительно хотите удалить доску №%1(%2)?").arg(num, type)))
        return;

    model->removeRow(index.row(), QModelIndex());
}

void DslamPageWidget::getBoardList()
{
    BoardTableModel *model = mDevice.objectCast<Dslam>()->boardTableModel();

    if (!model->getBoardListFromDevice())
        showMessage(model->error(), ImprovedMessageWidget::Error);
}

void DslamPageWidget::renumeringPairs()
{
    BoardTableModel *model = mDevice.objectCast<Dslam>()->boardTableModel();

    model->renumeringPairList();
}

void DslamPageWidget::beginEditBoardList()
{
    emit modeChanged(true);
}

void DslamPageWidget::finishEditBoardList()
{
    emit modeChanged(false);
}

void DslamPageWidget::pageModeChanged(bool editMode)
{
    mEditMode = editMode;
    ui->beginEditButton->setVisible(!editMode);
    ui->finishEditButton->setVisible(editMode);
    ui->editToolButtonPanel->setVisible(editMode);

    QItemSelectionModel *model = ui->dslamTreeView->selectionModel();
    model->clearCurrentIndex();
    model->clearSelection();

    if (editMode) {
        connect(model, &QItemSelectionModel::currentColumnChanged,
                this, &DslamPageWidget::currentColumnChanged);
        currentColumnChanged(QModelIndex(), QModelIndex());
    } else {
        disconnect(model, &QItemSelectionModel::currentColumnChanged,
                this, &DslamPageWidget::currentColumnChanged);
    }

    setupMenu();
    setupView();
}

void DslamPageWidget::currentColumnChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    if(!current.isValid()) {
        ui->editBoardAction->setEnabled(false);
        ui->removeBoardAction->setEnabled(false);\
        return;
    }

    bool autoNumeringBoard  = mDevice.objectCast<Dslam>()->autoNumeringBoard() == 1 ? true : false;
    bool autoFill  = mDevice.objectCast<Dslam>()->autoFill() == 1 ? true : false;

    if (autoFill) {
        ui->removeBoardAction->setEnabled(false);

        if ((!autoNumeringBoard) && (current.column() == 2)) {
            ui->editBoardAction->setEnabled(true);
        } else {
            ui->editBoardAction->setEnabled(false);
        }
    } else {
        ui->editBoardAction->setEnabled(current.column() != 0);
        ui->removeBoardAction->setEnabled(true);
    }
}

void DslamPageWidget::setupMenu()
{
    mContextMenu->clear();

    if (mEditMode) {
        mContextMenu->addAction(ui->editBoardAction);
        mContextMenu->addAction(ui->removeBoardAction);
        mContextMenu->addSeparator();
        mContextMenu->addAction(ui->getBoardListAction);
        mContextMenu->addAction(ui->renumeringPairAction);
    } else if (qstrcmp(ui->dslamTreeView->model()->metaObject()->className(), "BoardTableModel") == 0) {
        mContextMenu->addAction(ui->showBoardAction);
    } else {
        mContextMenu->addAction(ui->refreshPortInfoAction);
        mContextMenu->addAction(ui->editPortDescAction);
        mContextMenu->addSeparator();
        mContextMenu->addAction(ui->upPortAction);
        mContextMenu->addAction(ui->downPortAction);
        mContextMenu->addAction(ui->showProfileFrameAction);
        mContextMenu->addSeparator();
        mContextMenu->addAction(ui->collapseAllNodeAction);
        mContextMenu->addAction(ui->refreshAllPortInfoAction);
    }
}

void DslamPageWidget::setupView()
{
    if (mEditMode) {
        disconnect(ui->dslamTreeView, &QTreeView::doubleClicked,
                this, &DslamPageWidget::showPortListModel);
        ui->dslamTreeView->setSelectionBehavior(QAbstractItemView::SelectItems);
        ui->dslamTreeView->setEditTriggers(QAbstractItemView::DoubleClicked
                                           | QAbstractItemView::EditKeyPressed);
        ui->dslamTreeView->setItemDelegate(mBoardTableDelegate);
    } else {
        connect(ui->dslamTreeView, &QTreeView::doubleClicked,
                this, &DslamPageWidget::showPortListModel);
        ui->dslamTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->dslamTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->dslamTreeView->setItemDelegate(new QItemDelegate(this));
    }
}

void DslamPageWidget::viewRequestContextMenu(QPoint point)
{
    mContextMenu->exec(ui->dslamTreeView->mapToGlobal(point));
}

void DslamPageWidget::portListExpandedNode(QModelIndex index)
{
    if (!index.isValid())
        return;

    if (index.internalId() != invalidParentIndex)
        return;

    DslamPortTableModel *portListModel = qobject_cast<DslamPortTableModel *>(ui->dslamTreeView->model());

    if (!portListModel->updatePortExtended(index))
        showMessage(trUtf8("Не удалось обновить информацию по порту."),
                    ImprovedMessageWidget::Error, portListModel->error());
}

void DslamPageWidget::updatePortTableFinished(bool withErrors)
{
    ui->updatePortFrame->hide();

    if (withErrors) {
        DslamPortTableModel *model = qobject_cast<DslamPortTableModel *>(ui->dslamTreeView->model());
        showMessage(trUtf8("Обновление информации по портам завершилось с ошибками."),
                    ImprovedMessageWidget::Error, model->error());
    } else {
        showMessage(trUtf8("Обновление информации по всем портам завершилось успешно."),
                    ImprovedMessageWidget::Positive);
    }
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
