#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "basicdialogs.h"
#include "config.h"
#include "constant.h"
#include "converters.h"
#include "gui/devicetablepagewidget.h"
#include "gui/dslampagewidget.h"
#include "gui/settingspagewidget.h"
#include "gui/switchpagewidget.h"
#include "gui/aboutpagewidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init_snmp("snmpapp");
    SOCK_STARTUP;

    //Menu Actions
    connect(ui->exitAction, &QAction::triggered,
            this, &MainWindow::close);
    connect(ui->aboutAction, &QAction::triggered,
            this, &MainWindow::showAboutProgramPage);
    connect(ui->settingsAction, &QAction::triggered,
            this, &MainWindow::showSettingsPage);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested,
            this, &MainWindow::tabCloseRequested);
    connect(ui->tabWidget, &QTabWidget::currentChanged,
            this, &MainWindow::tabCurrentChanged);
    connect(ui->saveConfigSwitchAction, &QAction::triggered,
            this, &MainWindow::saveSwitchConfig);
    connect(ui->upPortAction, &QAction::triggered,
            this, &MainWindow::upDslPort);
    connect(ui->downPortAction, &QAction::triggered,
            this, &MainWindow::downDslPort);
    connect(ui->updateServiceDataAction, &QAction::triggered,
            this, &MainWindow::getServiceDataFromDevice);

    //Panels actions
    connect(ui->mainBarAction, &QAction::triggered,
            ui->mainToolBar, &QToolBar::setVisible);
    connect(ui->mainToolBar, &QToolBar::visibilityChanged,
            ui->mainBarAction, &QAction::setChecked);
    connect(ui->switchBarAction, &QAction::triggered,
            ui->switchToolBar, &QToolBar::setVisible);
    connect(ui->switchToolBar, &QToolBar::visibilityChanged,
            ui->switchBarAction, &QAction::setChecked);
    connect(ui->dslamBarAction, &QAction::triggered,
            ui->dslamToolBar, &QToolBar::setVisible);
    connect(ui->dslamToolBar, &QToolBar::visibilityChanged,
            ui->dslamBarAction, &QAction::setChecked);

    ui->menuSwitch->setEnabled(false);
    ui->menuDslam->setEnabled(false);
    ui->menuOlt->setEnabled(false);

    this->setWindowState(Qt::WindowMaximized);

    mTypePageList = new QVector<PageType::Enum>();
    mPageList = new QHash<QString, QWidget *>();
}

MainWindow::~MainWindow()
{
    SOCK_CLEANUP;

    delete ui;
}

void MainWindow::createDeviceListPage()
{
    DeviceTablePageWidget *deviceListPage = new DeviceTablePageWidget(ui->tabWidget, mTypePageList, mPageList, this);
    deviceListPage->setObjectName(QString::fromUtf8("deviceListTab"));

    connect(ui->openDeviceAction, &QAction::triggered,
            deviceListPage, &DeviceTablePageWidget::openDevice);
    connect(ui->addDeviceAction, &QAction::triggered,
            deviceListPage, &DeviceTablePageWidget::addDevice);
    connect(ui->editDeviceAction, &QAction::triggered,
            deviceListPage, &DeviceTablePageWidget::editDevice);
    connect(ui->removeDeviceAction, &QAction::triggered,
            deviceListPage, &DeviceTablePageWidget::removeDevice);
    connect(ui->loadDeviceListAction, &QAction::triggered,
            deviceListPage, &DeviceTablePageWidget::loadDeviceList);
    connect(ui->saveDeviceListAction, &QAction::triggered,
            deviceListPage, &DeviceTablePageWidget::saveDeviceList);
    connect(ui->updateVlanAllSwitchAction, &QAction::triggered,
            deviceListPage, &DeviceTablePageWidget::batchUpdateVlansSwitch);
    connect(ui->updateAllDslamBoardsInfoAction, &QAction::triggered,
            deviceListPage, &DeviceTablePageWidget::batchUpdateBoardsDslam);
    connect(ui->updateInfoAllDevicesAction, &QAction::triggered,
            deviceListPage, &DeviceTablePageWidget::batchUpdateInfoAllDevices);
    connect(ui->updateAllProfileOltInfoAction, &QAction::triggered,
            deviceListPage, &DeviceTablePageWidget::batchUpdateProfilesOlt);
    connect(ui->editDslamBoardListAction, &QAction::triggered,
            deviceListPage, &DeviceTablePageWidget::showEditDslamBoardListPage);
    connect(ui->showVlanSwitchAction, &QAction::triggered,
            deviceListPage, &DeviceTablePageWidget::showVlanInfoGroupBox);
    connect(deviceListPage, &DeviceTablePageWidget::changedActiveItem,
            this, &MainWindow::deviceViewActivatedItem);

    mPageList->insert(deviceListPage->objectName(), deviceListPage);
    mTypePageList->push_back(PageType::DeviceListPage);

    ui->tabWidget->addTab(deviceListPage, QString::fromUtf8("Оборудование"));
    ui->tabWidget->setCurrentWidget(deviceListPage);
}

void MainWindow::loadDeviceList()
{
    ui->loadDeviceListAction->trigger();
}

void MainWindow::loadProgramSettings()
{
    if (!Config::load()) {
        BasicDialogs::error(this, BasicDialogStrings::Error, Config::error());
    }
}

void MainWindow::upDslPort()
{
    if (mTypePageList->at(ui->tabWidget->currentIndex()) != PageType::DslamPage) {
        return;
    }

    DslamPageWidget *dslamPageWidget = qobject_cast<DslamPageWidget *>(ui->tabWidget->currentWidget());

    dslamPageWidget->upDslPort();
}

void MainWindow::downDslPort()
{
    if (mTypePageList->at(ui->tabWidget->currentIndex()) != PageType::DslamPage)
        return;

    DslamPageWidget *dslamPageWidget = qobject_cast<DslamPageWidget *>(ui->tabWidget->currentWidget());

    dslamPageWidget->downDslPort();
}

void MainWindow::saveSwitchConfig()
{
    if (mTypePageList->at(ui->tabWidget->currentIndex()) != PageType::SwitchPage) {
        return;
    }

    SwitchPageWidget *switchPageWidget = qobject_cast<SwitchPageWidget *>(ui->tabWidget->currentWidget());

    switchPageWidget->saveSwitchConfig();
}

void MainWindow::getServiceDataFromDevice()
{
    int pageIndex = ui->tabWidget->currentIndex();
    PageType::Enum currentPageType = mTypePageList->at(pageIndex);

    if (currentPageType == PageType::DeviceListPage) {
        DeviceTablePageWidget *widget = qobject_cast<DeviceTablePageWidget *>(ui->tabWidget->currentWidget());
        widget->getServiceDataFromCurrentDevice();
    } else if ((currentPageType == PageType::DslamPage)
               || (currentPageType == PageType::SwitchPage)
               || (currentPageType == PageType::OltPage)) {
        PageWidget *widget = qobject_cast<PageWidget *>(ui->tabWidget->currentWidget());

        bool result = widget->device()->getServiceDataFromDevice();

        if (!result) {
            BasicDialogs::error(this, BasicDialogStrings::Error, widget->device()->error());
        } else {
            BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Информация c устройства получена."));
        }
    }
}

void MainWindow::showAboutProgramPage()
{
    if (mPageList->contains("aboutTab")) {
        ui->tabWidget->setCurrentWidget(mPageList->value("aboutTab"));
        return;
    }

    AboutPageWidget *aboutTab = new AboutPageWidget(this);
    aboutTab->setObjectName(QString::fromUtf8("aboutTab"));

    mPageList->insert(aboutTab->objectName(), aboutTab);
    mTypePageList->push_back(PageType::AboutPage);

    ui->tabWidget->addTab(aboutTab, QString::fromUtf8("О программе"));
    ui->tabWidget->setCurrentWidget(aboutTab);
}

void MainWindow::showSettingsPage()
{
    if (mPageList->contains("settingsTab")) {
        ui->tabWidget->setCurrentWidget(mPageList->value("settingsTab"));
        return;
    }

    SettingsPageWidget *settingsTab = new SettingsPageWidget(this);
    settingsTab->setObjectName(QString::fromUtf8("settingsTab"));

    mPageList->insert(settingsTab->objectName(), settingsTab);
    mTypePageList->push_back(PageType::SettingsPage);

    ui->tabWidget->addTab(settingsTab, QString::fromUtf8("Настройки"));
    ui->tabWidget->setCurrentWidget(settingsTab);
}

void MainWindow::tabCurrentChanged(int index)
{
    PageType::Enum pageType = mTypePageList->at(ui->tabWidget->currentIndex());

    ui->mainToolBar->setEnabled(pageType == PageType::DeviceListPage);
    ui->dslamToolBar->setEnabled(pageType == PageType::DslamPage);
    ui->switchToolBar->setEnabled(pageType == PageType::SwitchPage);
    ui->openDeviceAction->setEnabled(pageType == PageType::DeviceListPage);
    ui->addDeviceAction->setEnabled(pageType == PageType::DeviceListPage);
    ui->editDeviceAction->setEnabled(pageType == PageType::DeviceListPage);
    ui->removeDeviceAction->setEnabled(pageType == PageType::DeviceListPage);

    if (index != 0) {
        ui->menuDslam->setEnabled(false);
        ui->menuSwitch->setEnabled(false);
        ui->menuOlt->setEnabled(false);
    } else {
        DeviceTablePageWidget *deviceListPage = qobject_cast<DeviceTablePageWidget *>(mPageList->value("deviceListTab"));
        deviceListPage->clearSelection();
    }

    if ((pageType == PageType::DeviceListPage)
            || (pageType == PageType::DslamPage)
            || (pageType == PageType::DslamPage)
            || (pageType == PageType::DslamPage)) {
        ui->updateServiceDataAction->setEnabled(true);
    } else {
        ui->updateServiceDataAction->setEnabled(false);
    }
}

void MainWindow::tabCloseRequested(int index)
{
    if (index != 0) {
        QWidget *deletedWidget = ui->tabWidget->widget(index);
        ui->tabWidget->removeTab(index);
        mPageList->remove(deletedWidget->objectName());
        mTypePageList->remove(index);

        delete deletedWidget;
    }
}

void MainWindow::deviceViewActivatedItem(QModelIndex index)
{
    if (!index.isValid())
        return;

    DeviceTablePageWidget *deviceListPage = qobject_cast<DeviceTablePageWidget *>(mPageList->value("deviceListTab"));

    DeviceTableModel *deviceListModel = deviceListPage->deviceTableModel();
    index = deviceListPage->proxyModel()->mapToSource(index);

    QModelIndex deviceTypeIndex = deviceListModel->index(index.row(), 3);
    QString deviceTypeString = deviceListModel->data(deviceTypeIndex).toString();
    DeviceType::Enum deviceType = DeviceType::from(deviceTypeString);

    ui->menuDslam->setEnabled(deviceType == DeviceType::Dslam);
    ui->menuSwitch->setEnabled(deviceType == DeviceType::Switch);
    ui->menuOlt->setEnabled(deviceType == DeviceType::Olt);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    DeviceTablePageWidget *deviceListPage = qobject_cast<DeviceTablePageWidget *>(mPageList->value("deviceListTab"));
    DeviceTableModel *deviceListModel = deviceListPage->deviceTableModel();

    if (!deviceListModel->isModified()) {
        event->accept();
        DslamProfileConfig::deinit();
        return;
    }

    QScopedPointer<QMessageBox> boxOnClose(new QMessageBox(this));
    boxOnClose->setModal(true);
    boxOnClose->setIcon(QMessageBox::Question);
    boxOnClose->setWindowTitle(QString::fromUtf8("Запрос на сохранение"));
    boxOnClose->setText(QString::fromUtf8("Список устройств был отредактирован. Вы хотите сохранить изменения перед выходом?"));
    boxOnClose->setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    boxOnClose->exec();

    QMessageBox::ButtonRole result = boxOnClose->buttonRole(boxOnClose->clickedButton());

    if (result == QMessageBox::RejectRole) {
        event->ignore();
    } else if (result == QMessageBox::YesRole) {
        if (!deviceListModel->save()) {
            BasicDialogs::warning(this, QString::fromUtf8("Сохранение данных"), QString::fromUtf8("Ошибка: сохранение списка устройств не удалось."), deviceListModel->error());
        } else {
            BasicDialogs::information(this, QString::fromUtf8("Сохранение данных"), QString::fromUtf8("Информация: список устройств сохранен."));
        }

        event->accept();
        DslamProfileConfig::deinit();
    }
}
