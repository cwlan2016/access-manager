#include "devicelistpagewidget.h"
#include "ui_devicelistpagewidget.h"

DeviceListPageWidget::DeviceListPageWidget(QTabWidget* parentTabWidget, QVector<PageType>* typePageList, QHash<QString, QWidget *>* pageList, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::DeviceListPageWidget),
    mDeviceListModel(new DeviceListModel(this)),
    mProxyModel(new QSortFilterProxyModel(this)),
    mTypePageList(typePageList),
    mPageList(pageList),
    mParentTabWidget(parentTabWidget)
{
    ui->setupUi(this);

    mProxyModel->setSourceModel(mDeviceListModel);

    ui->deviceListTableView->setModel(mProxyModel);
    ui->deviceListTableView->setColumnWidth(0, 200);
    ui->deviceListTableView->setColumnWidth(3, 150);

    DeviceListDelegate *deviceListDelegate = new DeviceListDelegate(this);
    deviceListDelegate->setIndexDeviceModel(1);
    ui->deviceListTableView->setItemDelegate(deviceListDelegate);

    connect(ui->addDeviceAction, SIGNAL(triggered()), SLOT(addDevice()));
    connect(ui->editDeviceAction, SIGNAL(triggered()), SLOT(editDevice()));
    connect(ui->removeDeviceAction, SIGNAL(triggered()), SLOT(removeDevice()));
    connect(ui->deviceListTableView, SIGNAL(doubleClicked(QModelIndex)), SLOT(showDevicePage()));
    connect(ui->deviceListTableView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), SLOT(viewActivatedItem(QModelIndex, QModelIndex)));
    connect(ui->deviceListTableView, SIGNAL(customContextMenuRequested(QPoint)), SLOT(deviceViewRequestContextMenu(QPoint)));
    connect(ui->selectDeviceAction, SIGNAL(triggered()), SLOT(showDevicePage()));
    connect(ui->filterNameLineEdit, SIGNAL(textChanged(QString)), SLOT(filterDeviceTextChanged(QString)));
    connect(ui->vlanInfoGroupBox, SIGNAL(toggled(bool)), SLOT(vlanInfoGBoxStateChanged(bool)));
    connect(ui->showVlanSwitchAction, SIGNAL(triggered()), SLOT(showInfoVlanGroupBox()));
    connect(ui->editDslamBoardListAction, SIGNAL(triggered()), SLOT(showEditDslamBoardListPage()));

    ui->vlanInfoGroupBox->setHidden(true);
}

DeviceListPageWidget::~DeviceListPageWidget()
{
    delete ui;
}

DeviceListModel* DeviceListPageWidget::deviceListModel() const
{
    return mDeviceListModel;
}

QSortFilterProxyModel* DeviceListPageWidget::proxyModel() const
{
    return mProxyModel;
}

QModelIndex DeviceListPageWidget::currentDeviceListItem() const
{
    return ui->deviceListTableView->currentIndex();
}

void DeviceListPageWidget::clearSelection()
{
    ui->deviceListTableView->selectionModel()->clear();
}

void DeviceListPageWidget::batchUpdate(DeviceType updatingDeviceType)
{
    QString errorString = "";
    std::unique_ptr<QProgressDialog> progressDialog(new QProgressDialog(this));
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->setFixedWidth(300);
    progressDialog->setMaximum(mDeviceListModel->rowCount(QModelIndex()));

    if (updatingDeviceType == DeviceType::Dslam)
        progressDialog->setLabelText(BatchUpdateLabel::Dslam);
    else if (updatingDeviceType == DeviceType::Switch)
        progressDialog->setLabelText(BatchUpdateLabel::Switch);
    else
        progressDialog->setLabelText(BatchUpdateLabel::AllDevices);

    int size = mDeviceListModel->rowCount(QModelIndex());
    std::vector<DeviceInfo::Ptr> &deviceList = mDeviceListModel->deviceList();

    for (int i = 0; i < size; i++)
    {
        progressDialog->setValue(i);

        if (progressDialog->wasCanceled())
            break;

        DeviceType deviceType = deviceList[i]->deviceType();

        if ((deviceType == updatingDeviceType)
                || (updatingDeviceType == DeviceType::Other))
        {
            bool result = deviceList[i]->getServiceDataFromDevice();

            if (!result)
                errorString += deviceList.at(i)->error() + "\n";

            if (deviceType == DeviceType::Dslam)
            {
                if (std::static_pointer_cast<DslamInfo>(deviceList[i])->autoNumeringBoard())
                    std::static_pointer_cast<DslamInfo>(deviceList[i])->boardListModel()->renumeringPairList();
            }
        }
    }

    if (!errorString.isEmpty())
        BasicDialogs::error(this, BasicDialogTitle::Error,QString::fromUtf8("Во время обновления произошли ошибки!"), errorString);

}

void DeviceListPageWidget::loadDeviceList()
{
    if (mDeviceListModel->isModified())
    {
        if (!BasicDialogs::question(this, QString::fromUtf8("Повторная загрузка данных"),
                                             QString::fromUtf8("Все сделанные изменения будут утеряны. Вы хотите продолжить?")))
            return;
    }

    if (!mDeviceListModel->load())
    {
        BasicDialogs::warning(this, QString::fromUtf8("Загрузка данных"), QString::fromUtf8("Ошибка: загрузка списка устройств не удалась."), mDeviceListModel->error());
    }

    ui->deviceListTableView->sortByColumn(0, Qt::AscendingOrder);
}

void DeviceListPageWidget::saveDeviceList()
{
    if (!mDeviceListModel->save())
    {
        BasicDialogs::warning(this, QString::fromUtf8("Сохранение данных"), QString::fromUtf8("Ошибка: сохранение списка устройств не удалось."), mDeviceListModel->error());
    }
    else
    {
        BasicDialogs::information(this, QString::fromUtf8("Сохранение данных"), QString::fromUtf8("Информация: список устройств сохранен."));
    }
}

void DeviceListPageWidget::addDevice()
{
    ui->filterNameLineEdit->clear();

    int row = mDeviceListModel->rowCount(QModelIndex());
    mDeviceListModel->insertRow(row, QModelIndex());

    QModelIndex index = mProxyModel->mapFromSource(mDeviceListModel->index(row, 0));
    ui->deviceListTableView->scrollToBottom();
    ui->deviceListTableView->setCurrentIndex(index);
    ui->deviceListTableView->setFocus();
    ui->deviceListTableView->edit(index);
}

void DeviceListPageWidget::editDevice()
{
    ui->deviceListTableView->setFocus();
    ui->deviceListTableView->edit(ui->deviceListTableView->currentIndex());
}

void DeviceListPageWidget::removeDevice()
{
    QModelIndex index = mProxyModel->mapToSource(ui->deviceListTableView->currentIndex());

    if (!index.isValid())
        return;

    QString name = mDeviceListModel->data(mDeviceListModel->index(index.row(), 0)).toString();
    QString ip = mDeviceListModel->data(mDeviceListModel->index(index.row(), 2)).toString();

    if (!BasicDialogs::okToDelete(this, QString::fromUtf8("Удаление устройства"),
                                 QString(QString::fromUtf8("Вы действительно хотите удалить %1(%2)?")).arg(name, ip)))
        return;

    mDeviceListModel->removeRow(index.row(), QModelIndex());
}

void DeviceListPageWidget::updateVlanInfoSwitch()
{
    QModelIndex index = mProxyModel->mapToSource(ui->deviceListTableView->currentIndex());

    if (!index.isValid())
        return;

    bool result = mDeviceListModel->getVlanTagFromDevice(index);

    if (!result)
        BasicDialogs::error(this, BasicDialogTitle::Error, mDeviceListModel->error());
    else
    {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Информация о вланах обновлена."));

        if(ui->vlanInfoGroupBox->isVisible())
            showInfoVlanGroupBox();
    }
}

void DeviceListPageWidget::updateBoardInfoDslam()
{
    QModelIndex index = mProxyModel->mapToSource(ui->deviceListTableView->currentIndex());

    if (!index.isValid())
        return;

    bool result = mDeviceListModel->getBoardListFromDevice(index);

    if (!result)
        BasicDialogs::error(this, BasicDialogTitle::Error, mDeviceListModel->error());
    else
    {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Информация о досках обновлена."));
    }
}

void DeviceListPageWidget::batchUpdateBoardsInfoDslam()
{
    batchUpdate(DeviceType::Dslam);
}

void DeviceListPageWidget::batchUpdateVlanInfoSwitch()
{
    batchUpdate(DeviceType::Switch);
}

void DeviceListPageWidget::batchUpdateInfoAllDevices()
{
    batchUpdate(DeviceType::Other);
}

void DeviceListPageWidget::filterDeviceTextChanged(QString text)
{
    mProxyModel->setFilterWildcard(text % "*");
}

void DeviceListPageWidget::deviceViewRequestContextMenu(QPoint point)
{
    QMenu contextMenu(this);
    contextMenu.addAction(ui->selectDeviceAction);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->addDeviceAction);
    contextMenu.addAction(ui->editDeviceAction);
    contextMenu.addAction(ui->removeDeviceAction);

    QModelIndex devModelIndex = mProxyModel->mapToSource(ui->deviceListTableView->currentIndex());
    QModelIndex index = mDeviceListModel->index(devModelIndex.row(), 3);
    DeviceType deviceType = DeviceTypeFromString(mDeviceListModel->data(index).toString());

    if (deviceType == DeviceType::Switch)
    {
        contextMenu.addSeparator();
        contextMenu.addAction(ui->showVlanSwitchAction);
    }
    else if (deviceType == DeviceType::Dslam)
    {
        contextMenu.addSeparator();
        contextMenu.addAction(ui->editDslamBoardListAction);
    }

    contextMenu.exec(ui->deviceListTableView->mapToGlobal(point));
}

void DeviceListPageWidget::showInfoVlanGroupBox()
{
    QModelIndex index = mProxyModel->mapToSource(ui->deviceListTableView->currentIndex());

    if (!index.isValid())
        return;

    ui->inetVlanValueLabel->setText(QString::number(mDeviceListModel->inetVlan(index)));
    ui->iptvVlanValueLabel->setText(QString::number(mDeviceListModel->iptvVlan(index)));

    QString name = mDeviceListModel->data(mDeviceListModel->index(index.row(), 0)).toString();
    QString model = mDeviceListModel->data(mDeviceListModel->index(index.row(), 1)).toString();

    ui->vlanInfoGroupBox->setTitle(model % " " % name);
    ui->vlanInfoGroupBox->setChecked(true);
}

void DeviceListPageWidget::vlanInfoGBoxStateChanged(bool state)
{
    ui->vlanInfoGroupBox->setShown(state);
}

void DeviceListPageWidget::showDevicePage()
{
    QModelIndex index = ui->deviceListTableView->currentIndex();

    if (!index.isValid())
        return;

    index = mProxyModel->mapToSource(index);

    QString ip = mDeviceListModel->data(mDeviceListModel->index(index.row(), 2)).toString();
    QString typeString = mDeviceListModel->data(mDeviceListModel->index(index.row(), 3)).toString();
    QString namePage = typeString % ip;

    if (mPageList->contains(namePage))
    {
        mParentTabWidget->setCurrentWidget(mPageList->value(namePage));
        return;
    }

    QString deviceModelString = mDeviceListModel->data(mDeviceListModel->index(index.row(), 1)).toString();
    DeviceType deviceType = DeviceTypeFromString(typeString);

    QWidget* pageWidget;

    DeviceInfo::Ptr deviceInfo = mDeviceListModel->deviceList()[index.row()];

    if (deviceType == DeviceType::Switch)
    {
        pageWidget = new SwitchPageWidget(deviceInfo);
        pageWidget->setObjectName(namePage);
        mTypePageList->append(PageType::SwitchPage);
    }
    else if (deviceType == DeviceType::Dslam)
    {
        pageWidget = new DslamPageWidget(deviceInfo);
        pageWidget->setObjectName(namePage);
        mTypePageList->append(PageType::DslamPage);
    }
    else
    {
        return;
    }

    mPageList->insert(namePage, pageWidget);

    QString name = mDeviceListModel->data(mDeviceListModel->index(index.row(), 0)).toString();

    mParentTabWidget->addTab(pageWidget, deviceModelString % " " % name);
    mParentTabWidget->setCurrentWidget(pageWidget);
}

void DeviceListPageWidget::showEditDslamBoardListPage()
{
    QModelIndex index = ui->deviceListTableView->currentIndex();

    if (!index.isValid())
        return;

    index = mProxyModel->mapToSource(index);

    QString ip = mDeviceListModel->data(mDeviceListModel->index(index.row(), 2)).toString();
    QString typeString = mDeviceListModel->data(mDeviceListModel->index(index.row(), 3)).toString();
    QString namePage = typeString % ip % "Edit";
    if (mPageList->contains(namePage))
    {
        mParentTabWidget->setCurrentWidget(mPageList->value(namePage));
        return;
    }

    QString deviceModelString = mDeviceListModel->data(mDeviceListModel->index(index.row(), 1)).toString();
    DeviceModel deviceModel = DeviceModelFromString(deviceModelString);

    if ((deviceModel == DeviceModel::Other)
            || (deviceModel == DeviceModel::MXA32)
            || (deviceModel == DeviceModel::MXA64))
    {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Данная модель дслама не имеет досок."));
        return;
    }

    DeviceInfo::Ptr deviceInfo = mDeviceListModel->deviceList()[index.row()];

    QWidget* pageWidget = new EditDslamBoardListPageWidget(deviceInfo, mDeviceListModel);

    mPageList->insert(namePage, pageWidget);
    mTypePageList->append(PageType::EditDslamPage);

    QString name = mDeviceListModel->data(mDeviceListModel->index(index.row(), 0)).toString();

    QString title = QString::fromUtf8("%1 %2 - Правка")
                    .arg(deviceModelString)
                    .arg(name);

    mParentTabWidget->addTab(pageWidget, title);
    mParentTabWidget->setCurrentWidget(pageWidget);
}

void DeviceListPageWidget::viewActivatedItem(QModelIndex currIndex, QModelIndex prevIndex)
{
    Q_UNUSED(prevIndex)

    emit changedActiveItem(currIndex);
}
