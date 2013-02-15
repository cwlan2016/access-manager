#include "devicetablepagewidget.h"
#include "ui_devicetablepagewidget.h"

#ifdef _MSC_VER
#include "../constant.h"
#include "../converters.h"
#include "../Models/boardtablemodel.h"
#include "../Models/devicetabledelegate.h"
#include "../Pages/dslampagewidget.h"
#include "../Pages/editdslamboardtablepagewidget.h"
#include "../Pages/oltpagewidget.h"
#include "../Pages/switchpagewidget.h"
#else
#include "constant.h"
#include "converters.h"
#include "Models/boardtablemodel.h"
#include "Models/devicetabledelegate.h"
#include "Pages/dslampagewidget.h"
#include "Pages/editdslamboardtablepagewidget.h"
#include "Pages/oltpagewidget.h"
#include "Pages/switchpagewidget.h"
#endif

DeviceTablePageWidget::DeviceTablePageWidget(QTabWidget *parentTabWidget,
                                             QVector<PageType::Enum> *typePageList,
                                             QHash<QString, QWidget *> *pageList,
                                             QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceTablePageWidget),
    mDeviceTableModel(new DeviceTableModel(this)),
    mProxyModel(new QSortFilterProxyModel(this)),
    mPageList(pageList),
    mTypePageList(typePageList),
    mParentTabWidget(parentTabWidget)
{
    ui->setupUi(this);

    mProxyModel->setSourceModel(mDeviceTableModel);
    mProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    ui->deviceListTableView->setModel(mProxyModel);
    ui->deviceListTableView->setColumnWidth(0, 200);
    ui->deviceListTableView->setColumnWidth(3, 150);

    DeviceTableDelegate *deviceListDelegate = new DeviceTableDelegate(this);
    deviceListDelegate->setIndexDeviceModel(1);
    ui->deviceListTableView->setItemDelegate(deviceListDelegate);

    connect(ui->selectDeviceAction, &QAction::triggered,
            this, &DeviceTablePageWidget::openDevice);
    connect(ui->addDeviceAction, &QAction::triggered,
            this, &DeviceTablePageWidget::addDevice);
    connect(ui->editDeviceAction, &QAction::triggered,
            this, &DeviceTablePageWidget::editDevice);
    connect(ui->removeDeviceAction, &QAction::triggered,
            this, &DeviceTablePageWidget::removeDevice);
    connect(ui->deviceListTableView, &QTableView::doubleClicked,
            this, &DeviceTablePageWidget::openDevice);
    connect(ui->deviceListTableView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &DeviceTablePageWidget::viewActivatedItem);
    connect(ui->deviceListTableView, &QTableView::customContextMenuRequested,
            this, &DeviceTablePageWidget::deviceViewRequestContextMenu);
    connect(ui->filterNameLineEdit, &QLineEdit::textChanged,
            this, &DeviceTablePageWidget::filterDeviceTextChanged);
    connect(ui->vlanInfoGroupBox, &QGroupBox::toggled,
            this, &DeviceTablePageWidget::vlanInfoGBoxStateChanged);
    connect(ui->profileInfoGroupBox, &QGroupBox::toggled,
            this, &DeviceTablePageWidget::profileInfoGboxStateChanged);
    connect(ui->showVlanSwitchAction, &QAction::triggered,
            this, &DeviceTablePageWidget::showVlanInfoGroupBox);
    connect(ui->editDslamBoardListAction, &QAction::triggered,
            this, &DeviceTablePageWidget::showEditDslamBoardListPage);
    connect(ui->showProfilesOltAction, &QAction::triggered,
            this, &DeviceTablePageWidget::showProfileInfoGroupBox);

    ui->vlanInfoGroupBox->setVisible(false);
    ui->profileInfoGroupBox->setVisible(false);
}

DeviceTablePageWidget::~DeviceTablePageWidget()
{
    delete ui;
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

    QWidget *pageWidget;

    DeviceInfo::Ptr deviceInfo = mDeviceTableModel->deviceList()[index.row()];

    if (deviceType == DeviceType::Switch) {
        pageWidget = new SwitchPageWidget(deviceInfo, this);
        pageWidget->setObjectName(namePage);
        mTypePageList->push_back(PageType::SwitchPage);
    } else if (deviceType == DeviceType::Dslam) {
        pageWidget = new DslamPageWidget(deviceInfo, this);
        pageWidget->setObjectName(namePage);
        mTypePageList->push_back(PageType::DslamPage);
    } else if (deviceType == DeviceType::Olt) {
        pageWidget = new OltPageWidget(deviceInfo, this);
        pageWidget->setObjectName(namePage);
        mTypePageList->push_back(PageType::OltPage);
    } else {
        return;
    }

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

void DeviceTablePageWidget::updateVlanInfoSwitch()
{
    QModelIndex index = mProxyModel->mapToSource(ui->deviceListTableView->currentIndex());

    if (!index.isValid())
        return;

    bool result = mDeviceTableModel->getVlanTagFromDevice(index);

    if (!result) {
        BasicDialogs::error(this, BasicDialogStrings::Error, mDeviceTableModel->error());
    } else {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Информация о вланах обновлена."));

        if (ui->vlanInfoGroupBox->isVisible())
            showVlanInfoGroupBox();
    }
}

void DeviceTablePageWidget::updateBoardInfoDslam()
{
    QModelIndex index = mProxyModel->mapToSource(ui->deviceListTableView->currentIndex());

    if (!index.isValid())
        return;

    bool result = mDeviceTableModel->getBoardListFromDevice(index);

    if (!result) {
        BasicDialogs::error(this, BasicDialogStrings::Error, mDeviceTableModel->error());
    } else {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Информация о досках обновлена."));
    }
}

void DeviceTablePageWidget::updateProfileInfoOlt()
{
    QModelIndex index = mProxyModel->mapToSource(ui->deviceListTableView->currentIndex());

    if (!index.isValid())
        return;

    bool result = mDeviceTableModel->getProfilesFromDevice(index);

    if (!result) {
        BasicDialogs::error(this, BasicDialogStrings::Error, mDeviceTableModel->error());
    } else {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Информация о профилях обновлена."));
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

void DeviceTablePageWidget::showEditDslamBoardListPage()
{
    QModelIndex index = ui->deviceListTableView->currentIndex();

    if (!index.isValid())
        return;

    index = mProxyModel->mapToSource(index);

    QModelIndex ipIndex = mDeviceTableModel->index(index.row(), 2);
    QString ip = mDeviceTableModel->data(ipIndex).toString();
    QModelIndex typeIndex = mDeviceTableModel->index(index.row(), 3);
    QString typeString = mDeviceTableModel->data(typeIndex).toString();
    QString namePage = typeString % ip % " Edit";

    if (mPageList->contains(namePage)) {
        mParentTabWidget->setCurrentWidget(mPageList->value(namePage));
        return;
    }

    QModelIndex deviceModelIndex = mDeviceTableModel->index(index.row(), 1);
    QString deviceModelString = mDeviceTableModel->data(deviceModelIndex).toString();
    DeviceModel::Enum deviceModel = DeviceModel::from(deviceModelString);

    if ((deviceModel == DeviceModel::Other)
            || (deviceModel == DeviceModel::MXA32)
            || (deviceModel == DeviceModel::MXA64)) {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Данная модель дслама не имеет досок."));
        return;
    }

    DeviceInfo::Ptr deviceInfo = mDeviceTableModel->deviceList()[index.row()];

    QWidget *pageWidget = new EditDslamBoardTablePageWidget(deviceInfo, mDeviceTableModel, this);
    pageWidget->setObjectName(namePage);

    mPageList->insert(namePage, pageWidget);
    mTypePageList->push_back(PageType::EditDslamPage);

    QModelIndex nameIndex = mDeviceTableModel->index(index.row(), 0);
    QString name = mDeviceTableModel->data(nameIndex).toString();

    QString title = QString::fromUtf8("%1 %2 - Правка")
                    .arg(deviceModelString)
                    .arg(name);

    mParentTabWidget->addTab(pageWidget, title);
    mParentTabWidget->setCurrentWidget(pageWidget);
}

void DeviceTablePageWidget::showVlanInfoGroupBox()
{
    QModelIndex index = mProxyModel->mapToSource(ui->deviceListTableView->currentIndex());

    if (!index.isValid())
        return;

    ui->inetVlanValueLabel->setText(QString::number(mDeviceTableModel->inetVlan(index)));
    ui->iptvVlanValueLabel->setText(QString::number(mDeviceTableModel->iptvVlan(index)));

    QString name = mDeviceTableModel->data(mDeviceTableModel->index(index.row(), 0)).toString();
    QString model = mDeviceTableModel->data(mDeviceTableModel->index(index.row(), 1)).toString();

    ui->vlanInfoGroupBox->setTitle(model % " " % name);
    ui->vlanInfoGroupBox->setChecked(true);
}

void DeviceTablePageWidget::showProfileInfoGroupBox()
{
    QModelIndex index = mProxyModel->mapToSource(ui->deviceListTableView->currentIndex());

    if (!index.isValid())
        return;

    ui->serviceProfileListView->setModel(mDeviceTableModel->serviceProfileOltListModel(index));
    ui->multicastProfileListView->setModel(mDeviceTableModel->multicastProfileOltListModel(index));

    QString name = mDeviceTableModel->data(mDeviceTableModel->index(index.row(), 0)).toString();
    QString model = mDeviceTableModel->data(mDeviceTableModel->index(index.row(), 1)).toString();

    ui->profileInfoGroupBox->setTitle(model % " " % name);
    ui->profileInfoGroupBox->setChecked(true);
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
    QVector<DeviceInfo::Ptr> &deviceList = mDeviceTableModel->deviceList();

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
                if (deviceList.at(i).objectCast<DslamInfo>()->autoNumeringBoard())
                    deviceList.at(i).objectCast<DslamInfo>()->boardTableModel()->renumeringPairList();
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

void DeviceTablePageWidget::vlanInfoGBoxStateChanged(bool state)
{
    ui->vlanInfoGroupBox->setVisible(state);
}

void DeviceTablePageWidget::profileInfoGboxStateChanged(bool state)
{
    ui->profileInfoGroupBox->setVisible(state);
}

void DeviceTablePageWidget::deviceViewRequestContextMenu(QPoint point)
{
    QMenu contextMenu(this);
    contextMenu.addAction(ui->selectDeviceAction);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->addDeviceAction);
    contextMenu.addAction(ui->editDeviceAction);
    contextMenu.addAction(ui->removeDeviceAction);

    QModelIndex devModelIndex = mProxyModel->mapToSource(ui->deviceListTableView->currentIndex());
    QModelIndex index = mDeviceTableModel->index(devModelIndex.row(), 3);
    DeviceType::Enum deviceType = DeviceType::from(mDeviceTableModel->data(index).toString());

    if (deviceType == DeviceType::Switch) {
        contextMenu.addSeparator();
        contextMenu.addAction(ui->showVlanSwitchAction);
    } else if (deviceType == DeviceType::Dslam) {
        contextMenu.addSeparator();
        contextMenu.addAction(ui->editDslamBoardListAction);
    } else if (deviceType == DeviceType::Olt) {
        contextMenu.addSeparator();
        contextMenu.addAction(ui->showProfilesOltAction);
    }

    contextMenu.exec(ui->deviceListTableView->mapToGlobal(point));
}

void DeviceTablePageWidget::viewActivatedItem(QModelIndex currIndex, QModelIndex prevIndex)
{
    Q_UNUSED(prevIndex)

    emit changedActiveItem(currIndex);
}
