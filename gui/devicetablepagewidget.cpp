#include "devicetablepagewidget.h"
#include "ui_devicetablepagewidget.h"

#include <constant.h>
#include <converters.h>
#include <models/boardtablemodel.h>
#include <models/devicetabledelegate.h>
#include <gui/dslampagewidget.h>
#include <gui/oltpagewidget.h>
#include <gui/switchpagewidget.h>

DeviceTablePageWidget::DeviceTablePageWidget(QTabWidget *parentTabWidget,
                                             QVector<PageType::Enum> *typePageList,
                                             QHash<QString, QWidget *> *pageList,
                                             QWidget *parent) :
    PageWidget(parent),
    ui(new Ui::DeviceTablePageWidget),
    mDeviceTableModel(new DeviceTableModel(ui->messageWidget, this)),
    mProxyModel(new QSortFilterProxyModel(this)),
    mPageList(pageList),
    mTypePageList(typePageList),
    mParentTabWidget(parentTabWidget)
{
    ui->setupUi(this);

    ui->messageWidget->hide();
    ui->messageWidget->setCloseButtonVisible(true);
}

DeviceTablePageWidget::~DeviceTablePageWidget()
{
    delete ui;
}

void DeviceTablePageWidget::init()
{
    initActions();
    initComponents();
    initMenu();
    initView();
}

void DeviceTablePageWidget::loadDeviceList()
{
    if (mDeviceTableModel->isModified()) {
        if (!BasicDialogs::question(this, QString::fromUtf8("Повторная загрузка данных"),
                                    QString::fromUtf8("Все сделанные изменения будут утеряны. Вы хотите продолжить?")))
            return;
    }

    if (!mDeviceTableModel->load()) {
        showMessage(trUtf8("Не удалось загрузить список устройств."), ImprovedMessageWidget::Warning, mDeviceTableModel->error());
    }

    ui->deviceListTableView->sortByColumn(DeviceTableModel::NameColumn, Qt::AscendingOrder);
}

void DeviceTablePageWidget::saveDeviceList()
{
    if (!mDeviceTableModel->save()) {
        showMessage(trUtf8("Не удалось сохранить список устройств."), ImprovedMessageWidget::Warning, mDeviceTableModel->error());
    } else {
        showMessage(trUtf8("Информация: список устройств сохранен."), ImprovedMessageWidget::Positive);
    }
}

void DeviceTablePageWidget::getServiceDataFromCurrentDevice()
{
    QModelIndex index = mProxyModel->mapToSource(ui->deviceListTableView->currentIndex());

    if (!index.isValid())
        return;

    bool result = mDeviceTableModel->getServiceDataFromDevice(index);

    if (!result) {
        showMessage(mDeviceTableModel->error(), ImprovedMessageWidget::Error);
    } else {
        showMessage(trUtf8("Информация c устройства получена."));
    }
}

void DeviceTablePageWidget::batchUpdateBoardsDslam()
{
    batchUpdate(DeviceType::Dslam);
}

void DeviceTablePageWidget::batchUpdateVlansSwitch()
{
    batchUpdate(DeviceType::Switch);
}

void DeviceTablePageWidget::batchUpdateProfilesOlt()
{
    batchUpdate(DeviceType::Olt);
}

void DeviceTablePageWidget::batchUpdateInfoAllDevices()
{
    batchUpdate(DeviceType::Other);
}

void DeviceTablePageWidget::clearSelection()
{
    ui->deviceListTableView->selectionModel()->clear();
}

DeviceTableModel *DeviceTablePageWidget::deviceTableModel() const
{
    return mDeviceTableModel;
}

void DeviceTablePageWidget::showMessage(const QString &msg, ImprovedMessageWidget::MessageType messageType, const QString &detailedText)
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

void DeviceTablePageWidget::openDevice()
{
    QModelIndex index = ui->deviceListTableView->currentIndex();

    if (!index.isValid())
        return;

    index = mProxyModel->mapToSource(index);

    QModelIndex ipIndex = mDeviceTableModel->index(index.row(), DeviceTableModel::IpColumn);
    QString ip = mDeviceTableModel->data(ipIndex).toString();
    QModelIndex typeIndex = mDeviceTableModel->index(index.row(), DeviceTableModel::DeviceTypeColumn);
    QString typeString = mDeviceTableModel->data(typeIndex).toString();
    QString namePage = typeString % ip;

    if (mPageList->contains(namePage)) {
        mParentTabWidget->setCurrentWidget(mPageList->value(namePage));
        return;
    }

    QModelIndex deviceModelIndex = mDeviceTableModel->index(index.row(), DeviceTableModel::DeviceModelColumn);
    QString deviceModelString = mDeviceTableModel->data(deviceModelIndex).toString();
    DeviceType::Enum deviceType = DeviceType::from(typeString);

    PageWidget *pageWidget;

    Device::Ptr deviceInfo = mDeviceTableModel->deviceList()[index.row()];

    if (deviceType == DeviceType::Switch) {
        pageWidget = new SwitchPageWidget(deviceInfo, this);
        mTypePageList->push_back(PageType::SwitchPage);
    } else if (deviceType == DeviceType::Dslam) {
        pageWidget = new DslamPageWidget(deviceInfo, this);
        mTypePageList->push_back(PageType::DslamPage);
    } else if (deviceType == DeviceType::Olt) {
        pageWidget = new OltPageWidget(deviceInfo, this);
        mTypePageList->push_back(PageType::OltPage);
    } else {
        return;
    }

    pageWidget->init();
    pageWidget->setObjectName(namePage);
    mPageList->insert(namePage, pageWidget);

    QString name = mDeviceTableModel->data(mDeviceTableModel->index(index.row(), DeviceTableModel::NameColumn)).toString();

    mParentTabWidget->addTab(pageWidget, deviceModelString % " " % name);
    mParentTabWidget->setCurrentWidget(pageWidget);
}

void DeviceTablePageWidget::addDevice()
{
    ui->filterNameLineEdit->clear();

    int row = mDeviceTableModel->rowCount(QModelIndex());
    mDeviceTableModel->insertRow(row, QModelIndex());

    QModelIndex index = mProxyModel->mapFromSource(mDeviceTableModel->index(row, DeviceTableModel::NameColumn));
    ui->deviceListTableView->scrollToBottom();
    ui->deviceListTableView->setCurrentIndex(index);
    ui->deviceListTableView->setFocus();
    ui->deviceListTableView->edit(index);
}

void DeviceTablePageWidget::editDevice()
{
    ui->deviceListTableView->setFocus();
    ui->deviceListTableView->edit(ui->deviceListTableView->currentIndex());
}

void DeviceTablePageWidget::removeDevice()
{
    QModelIndex index = mProxyModel->mapToSource(ui->deviceListTableView->currentIndex());

    if (!index.isValid())
        return;

    QModelIndex nameindex = mDeviceTableModel->index(index.row(), DeviceTableModel::NameColumn);
    QString name = mDeviceTableModel->data(nameindex).toString();
    QModelIndex ipIndex = mDeviceTableModel->index(index.row(), DeviceTableModel::IpColumn);
    QString ip = mDeviceTableModel->data(ipIndex).toString();

    if (!BasicDialogs::okToDelete(this, QString::fromUtf8("Удаление устройства"),
                                  QString(QString::fromUtf8("Вы действительно хотите удалить %1(%2)?")).arg(name, ip)))
        return;

    mDeviceTableModel->removeRow(index.row(), QModelIndex());
}

void DeviceTablePageWidget::showSwitchExtInfoFrame()
{
    QModelIndex index = mProxyModel->mapToSource(ui->deviceListTableView->currentIndex());

    if (!index.isValid())
        return;

    ui->inetVlanValueLabel->setText(QString::number(mDeviceTableModel->inetVlan(index)));
    ui->iptvVlanValueLabel->setText(QString::number(mDeviceTableModel->iptvVlan(index)));

    QString name = mDeviceTableModel->data(mDeviceTableModel->index(index.row(), DeviceTableModel::NameColumn)).toString();
    QString model = mDeviceTableModel->data(mDeviceTableModel->index(index.row(), DeviceTableModel::DeviceModelColumn)).toString();

    ui->switchExtInfoLabel->setText(QString::fromUtf8("Коммутатор ") % model % ", " % name);
    ui->switchExtInfoFrame->setVisible(true);
}

void DeviceTablePageWidget::showOltExtInfoFrame()
{
    QModelIndex index = mProxyModel->mapToSource(ui->deviceListTableView->currentIndex());

    if (!index.isValid())
        return;

    ui->serviceProfileListView->setModel(mDeviceTableModel->serviceProfileOltListModel(index));
    ui->multicastProfileListView->setModel(mDeviceTableModel->multicastProfileOltListModel(index));

    QString name = mDeviceTableModel->data(mDeviceTableModel->index(index.row(), DeviceTableModel::NameColumn)).toString();
    QString model = mDeviceTableModel->data(mDeviceTableModel->index(index.row(), DeviceTableModel::DeviceModelColumn)).toString();

    ui->oltExtInfoLabel->setText(QString::fromUtf8("Линейный оптический терминал ") % model % ", " % name);
    ui->oltExtInfoFrame->setVisible(true);
}

void DeviceTablePageWidget::showExtendedInfoPanel()
{
    QModelIndex currentIndex = mProxyModel->mapToSource(ui->deviceListTableView->currentIndex());
    QModelIndex deviceTypeIndex = mDeviceTableModel->index(currentIndex.row(), DeviceTableModel::DeviceTypeColumn);
    DeviceType::Enum deviceType = DeviceType::from(mDeviceTableModel->data(deviceTypeIndex).toString());

    if (deviceType == DeviceType::Switch) {
        showSwitchExtInfoFrame();
    } else if (deviceType == DeviceType::Dslam) {
        showMessage(trUtf8("Для данного типа устройства дополнительная информация недоступна."), ImprovedMessageWidget::Information);
    } else if (deviceType == DeviceType::Olt) {
        showOltExtInfoFrame();
    }
}

void DeviceTablePageWidget::initActions()
{
    connect(ui->openDeviceTabAction, &QAction::triggered,
            this, &DeviceTablePageWidget::openDevice);
    connect(ui->addDeviceAction, &QAction::triggered,
            this, &DeviceTablePageWidget::addDevice);
    connect(ui->editDeviceAction, &QAction::triggered,
            this, &DeviceTablePageWidget::editDevice);
    connect(ui->removeDeviceAction, &QAction::triggered,
            this, &DeviceTablePageWidget::removeDevice);
    connect(ui->extendedInfoAction, &QAction::triggered,
            this, &DeviceTablePageWidget::showExtendedInfoPanel);

    ui->openDeviceTabButton->setDefaultAction(ui->openDeviceTabAction);
    ui->addDeviceButton->setDefaultAction(ui->addDeviceAction);
    ui->editDeviceButton->setDefaultAction(ui->editDeviceAction);
    ui->removeDeviceButton->setDefaultAction(ui->removeDeviceAction);
    ui->extendedInfoButton->setDefaultAction(ui->extendedInfoAction);
}

void DeviceTablePageWidget::initComponents()
{
    connect(ui->filterNameLineEdit, &QLineEdit::textChanged,
            this, &DeviceTablePageWidget::filterDeviceTextChanged);
    connect(ui->closeSwitchExtInfoButton, &QToolButton::pressed,
            ui->switchExtInfoFrame, &QFrame::hide);
    connect(ui->closeOltExtInfoButton, &QToolButton::pressed,
            ui->oltExtInfoFrame, &QFrame::hide);

    ui->switchExtInfoFrame->setVisible(false);
    ui->oltExtInfoFrame->setVisible(false);
}

void DeviceTablePageWidget::initMenu()
{
    mContextMenu->addAction(ui->openDeviceTabAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(ui->addDeviceAction);
    mContextMenu->addAction(ui->editDeviceAction);
    mContextMenu->addAction(ui->removeDeviceAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(ui->extendedInfoAction);
}

void DeviceTablePageWidget::initView()
{
    mProxyModel->setSourceModel(mDeviceTableModel);
    mProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    ui->deviceListTableView->setModel(mProxyModel);
    ui->deviceListTableView->setColumnWidth(DeviceTableModel::NameColumn, 250);
    ui->deviceListTableView->setColumnWidth(DeviceTableModel::DeviceModelColumn, 100);
    ui->deviceListTableView->setColumnWidth(DeviceTableModel::IpColumn, 150);
    ui->deviceListTableView->setColumnWidth(DeviceTableModel::DeviceTypeColumn, 150);

    DeviceTableDelegate *deviceListDelegate = new DeviceTableDelegate(this);
    ui->deviceListTableView->setItemDelegate(deviceListDelegate);

    connect(ui->deviceListTableView, &QTableView::doubleClicked,
            this, &DeviceTablePageWidget::openDevice);
    connect(ui->deviceListTableView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &DeviceTablePageWidget::viewActivatedItem);
    connect(ui->deviceListTableView, &QTableView::customContextMenuRequested,
            this, &DeviceTablePageWidget::deviceViewRequestContextMenu);

    clearSelection();

    ui->editDeviceAction->setEnabled(false);
    ui->removeDeviceAction->setEnabled(false);
    ui->extendedInfoAction->setEnabled(false);
}

void DeviceTablePageWidget::batchUpdate(DeviceType::Enum updatingDeviceType)
{
    QScopedPointer<QProgressDialog> progressDialog(new QProgressDialog(this));
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->setFixedWidth(300);
    progressDialog->setMaximum(mDeviceTableModel->rowCount(QModelIndex()));

    if (updatingDeviceType == DeviceType::Dslam) {
        progressDialog->setLabelText(BatchUpdateStrings::Dslam);
    } else if (updatingDeviceType == DeviceType::Switch) {
        progressDialog->setLabelText(BatchUpdateStrings::Switch);
    } else if (updatingDeviceType == DeviceType::Olt) {
        progressDialog->setLabelText(BatchUpdateStrings::Olt);
    } else {
        progressDialog->setLabelText(BatchUpdateStrings::AllDevices);
    }

    int size = mDeviceTableModel->rowCount(QModelIndex());
    QVector<Device::Ptr> &deviceList = mDeviceTableModel->deviceList();

    QString errorString = "";

    for (int i = 0; i < size; ++i) {
        progressDialog->setValue(i);

        if (progressDialog->wasCanceled())
            break;

        DeviceType::Enum deviceType = deviceList[i]->deviceType();

        if ((deviceType == updatingDeviceType)
                || (updatingDeviceType == DeviceType::Other)) {
            bool result = deviceList[i]->getServiceDataFromDevice();

            if (!result)
                errorString += deviceList.at(i)->error() + "\n";

            if (deviceType == DeviceType::Dslam) {
                if (deviceList.at(i).objectCast<Dslam>()->autoNumeringBoard())
                    deviceList.at(i).objectCast<Dslam>()->boardTableModel()->renumeringPairList();
            }
        }
    }

    if (!errorString.isEmpty())
        showMessage(trUtf8("Во время обновления произошли ошибки!"), ImprovedMessageWidget::Error, errorString);
}

void DeviceTablePageWidget::filterDeviceTextChanged(QString text)
{
    mProxyModel->setFilterWildcard(text % "*");
}

void DeviceTablePageWidget::deviceViewRequestContextMenu(QPoint point)
{
    mContextMenu->exec(ui->deviceListTableView->mapToGlobal(point));
}

void DeviceTablePageWidget::viewActivatedItem(QModelIndex currIndex,
                                              QModelIndex prevIndex)
{
    Q_UNUSED(prevIndex)

    if (currIndex.isValid()) {
        Qt::ItemFlags flags = mProxyModel->flags(ui->deviceListTableView->currentIndex());
        ui->editDeviceAction->setEnabled(flags && Qt::ItemIsEditable);
        ui->removeDeviceAction->setEnabled(true);
        ui->extendedInfoAction->setEnabled(true);
    } else {
        ui->editDeviceAction->setEnabled(false);
        ui->removeDeviceAction->setEnabled(false);
        ui->extendedInfoAction->setEnabled(false);
    }
}
