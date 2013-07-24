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
    mDeviceTableModel(new DeviceTableModel(this)),
    mProxyModel(new QSortFilterProxyModel(this)),
    mPageList(pageList),
    mTypePageList(typePageList),
    mParentTabWidget(parentTabWidget)
{
    ui->setupUi(this);
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
        BasicDialogs::warning(this, QString::fromUtf8("Загрузка данных"), QString::fromUtf8("Ошибка: загрузка списка устройств не удалась."), mDeviceTableModel->error());
    }

    ui->deviceListTableView->sortByColumn(0, Qt::AscendingOrder);
}

void DeviceTablePageWidget::saveDeviceList()
{
    if (!mDeviceTableModel->save()) {
        BasicDialogs::warning(this, QString::fromUtf8("Сохранение данных"),
                              QString::fromUtf8("Ошибка: сохранение списка устройств не удалось."), mDeviceTableModel->error());
    } else {
        BasicDialogs::information(this, QString::fromUtf8("Сохранение данных"),
                                  QString::fromUtf8("Информация: список устройств сохранен."));
    }
}

void DeviceTablePageWidget::openDevice()
{
    QModelIndex index = ui->deviceListTableView->currentIndex();

    if (!index.isValid())
        return;

    index = mProxyModel->mapToSource(index);

    QModelIndex ipIndex = mDeviceTableModel->index(index.row(), 2);
    QString ip = mDeviceTableModel->data(ipIndex).toString();
    QModelIndex typeIndex = mDeviceTableModel->index(index.row(), 3);
    QString typeString = mDeviceTableModel->data(typeIndex).toString();
    QString namePage = typeString % ip;

    if (mPageList->contains(namePage)) {
        mParentTabWidget->setCurrentWidget(mPageList->value(namePage));
        return;
    }

    QModelIndex deviceModelIndex = mDeviceTableModel->index(index.row(), 1);
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

    QString name = mDeviceTableModel->data(mDeviceTableModel->index(index.row(), 0)).toString();

    mParentTabWidget->addTab(pageWidget, deviceModelString % " " % name);
    mParentTabWidget->setCurrentWidget(pageWidget);
}

void DeviceTablePageWidget::addDevice()
{
    ui->filterNameLineEdit->clear();

    int row = mDeviceTableModel->rowCount(QModelIndex());
    mDeviceTableModel->insertRow(row, QModelIndex());

    QModelIndex index = mProxyModel->mapFromSource(mDeviceTableModel->index(row, 0));
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

    QModelIndex nameindex = mDeviceTableModel->index(index.row(), 0);
    QString name = mDeviceTableModel->data(nameindex).toString();
    QModelIndex ipIndex = mDeviceTableModel->index(index.row(), 2);
    QString ip = mDeviceTableModel->data(ipIndex).toString();

    if (!BasicDialogs::okToDelete(this, QString::fromUtf8("Удаление устройства"),
                                  QString(QString::fromUtf8("Вы действительно хотите удалить %1(%2)?")).arg(name, ip)))
        return;

    mDeviceTableModel->removeRow(index.row(), QModelIndex());
}

void DeviceTablePageWidget::getServiceDataFromCurrentDevice()
{
    QModelIndex index = mProxyModel->mapToSource(ui->deviceListTableView->currentIndex());

    if (!index.isValid())
        return;

    bool result = mDeviceTableModel->getServiceDataFromDevice(index);

    if (!result) {
        BasicDialogs::error(this, BasicDialogStrings::Error, mDeviceTableModel->error());
    } else {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Информация c устройства получена."));
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

void DeviceTablePageWidget::showSwitchExtInfoFrame()
{
    QModelIndex index = mProxyModel->mapToSource(ui->deviceListTableView->currentIndex());

    if (!index.isValid())
        return;

    ui->inetVlanValueLabel->setText(QString::number(mDeviceTableModel->inetVlan(index)));
    ui->iptvVlanValueLabel->setText(QString::number(mDeviceTableModel->iptvVlan(index)));

    QString name = mDeviceTableModel->data(mDeviceTableModel->index(index.row(), 0)).toString();
    QString model = mDeviceTableModel->data(mDeviceTableModel->index(index.row(), 1)).toString();

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

    QString name = mDeviceTableModel->data(mDeviceTableModel->index(index.row(), 0)).toString();
    QString model = mDeviceTableModel->data(mDeviceTableModel->index(index.row(), 1)).toString();

    ui->oltExtInfoLabel->setText(QString::fromUtf8("Линейный оптический терминал ") % model % ", " % name);
    ui->oltExtInfoFrame->setVisible(true);
}

void DeviceTablePageWidget::showExtendedInfoPanel()
{
    QModelIndex currentIndex = mProxyModel->mapToSource(ui->deviceListTableView->currentIndex());
    QModelIndex deviceTypeIndex = mDeviceTableModel->index(currentIndex.row(), 3);
    DeviceType::Enum deviceType = DeviceType::from(mDeviceTableModel->data(deviceTypeIndex).toString());

    if (deviceType == DeviceType::Switch) {
        showSwitchExtInfoFrame();
    } else if (deviceType == DeviceType::Dslam) {
        BasicDialogs::information(this, BasicDialogStrings::Info,
                                  QString::fromUtf8("Для данного типа устройства дополнительная информация недоступна."));
    } else if (deviceType == DeviceType::Olt) {
        showOltExtInfoFrame();
    }
}

void DeviceTablePageWidget::clearSelection()
{
    ui->deviceListTableView->selectionModel()->clear();
}

DeviceTableModel *DeviceTablePageWidget::deviceTableModel() const
{
    return mDeviceTableModel;
}

QSortFilterProxyModel *DeviceTablePageWidget::proxyModel() const
{
    return mProxyModel;
}

QModelIndex DeviceTablePageWidget::currentDeviceListItem() const
{
    return ui->deviceListTableView->currentIndex();
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
    ui->deviceListTableView->setColumnWidth(0, 200);
    ui->deviceListTableView->setColumnWidth(1, 100);
    ui->deviceListTableView->setColumnWidth(2, 150);
    ui->deviceListTableView->setColumnWidth(3, 150);

    DeviceTableDelegate *deviceListDelegate = new DeviceTableDelegate(this);
    deviceListDelegate->setIndexDeviceModel(1);
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
    QString errorString = "";
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
        BasicDialogs::error(this, BasicDialogStrings::Error, QString::fromUtf8("Во время обновления произошли ошибки!"), errorString);
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
        ui->editDeviceAction->setEnabled(currIndex.column() != 3);
        ui->removeDeviceAction->setEnabled(true);
        ui->extendedInfoAction->setEnabled(true);
    } else {
        ui->editDeviceAction->setEnabled(false);
        ui->removeDeviceAction->setEnabled(false);
        ui->extendedInfoAction->setEnabled(false);
    }
}
