#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui/QCloseEvent>
#include <QtWidgets/QMessageBox>

#include "config.h"
#include "constant.h"
#include "converters.h"
#include "Pages/devicelistpagewidget.h"
#include "Pages/dslampagewidget.h"
#include "Pages/settingspagewidget.h"
#include "Pages/switchpagewidget.h"
#include "Pages/aboutpagewidget.h"

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
    DeviceListPageWidget *deviceListPage = new DeviceListPageWidget(ui->tabWidget, mTypePageList, mPageList, this);
    deviceListPage->setObjectName(QString::fromUtf8("deviceListTab"));

    connect(ui->openDeviceAction, &QAction::triggered,
            deviceListPage, &DeviceListPageWidget::openDevice);
    connect(ui->addDeviceAction, &QAction::triggered,
            deviceListPage, &DeviceListPageWidget::addDevice);
    connect(ui->editDeviceAction, &QAction::triggered,
            deviceListPage, &DeviceListPageWidget::editDevice);
    connect(ui->removeDeviceAction, &QAction::triggered,
            deviceListPage, &DeviceListPageWidget::removeDevice);
    connect(ui->loadDeviceListAction, &QAction::triggered,
            deviceListPage, &DeviceListPageWidget::loadDeviceList);
    connect(ui->saveDeviceListAction, &QAction::triggered,
            deviceListPage, &DeviceListPageWidget::saveDeviceList);
    connect(ui->updateVlanSwitchAction, &QAction::triggered,
            deviceListPage, &DeviceListPageWidget::updateVlanInfoSwitch);
    connect(ui->updateDslamBoardsInfoAction, &QAction::triggered,
            deviceListPage, &DeviceListPageWidget::updateBoardInfoDslam);
    connect(ui->updateProfilesOltAction, &QAction::triggered,
            deviceListPage, &DeviceListPageWidget::updateProfileInfoOlt);
    connect(ui->updateVlanAllSwitchAction, &QAction::triggered,
            deviceListPage, &DeviceListPageWidget::batchUpdateVlanInfoSwitch);
    connect(ui->updateAllDslamBoardsInfoAction, &QAction::triggered,
            deviceListPage, &DeviceListPageWidget::batchUpdateBoardsInfoDslam);
    connect(ui->updateInfoAllDevicesAction, &QAction::triggered,
            deviceListPage, &DeviceListPageWidget::batchUpdateInfoAllDevices);
    connect(ui->updateAllProfileOltInfoAction, &QAction::triggered,
            deviceListPage, &DeviceListPageWidget::batchUpdateProfileOlt);
    connect(ui->editDslamBoardListAction, &QAction::triggered,
            deviceListPage, &DeviceListPageWidget::showEditDslamBoardListPage);
    connect(ui->showVlanSwitchAction, &QAction::triggered,
            deviceListPage, &DeviceListPageWidget::showVlanInfoGroupBox);
    connect(deviceListPage, &DeviceListPageWidget::changedActiveItem,
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
        BasicDialogs::error(this, BasicDialogTitle::Error, Config::errorString());
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    DeviceListPageWidget *deviceListPage = qobject_cast<DeviceListPageWidget *>(mPageList->value("deviceListTab"));
    DeviceListModel *deviceListModel = deviceListPage->deviceListModel();

    if (!deviceListModel->isModified()) {
        event->accept();
        return;
    }

    std::unique_ptr<QMessageBox> boxOnClose(new QMessageBox(this));
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
    }
}

void MainWindow::showAboutProgramPage()
{
    if (mPageList->contains("aboutTab")) {
        ui->tabWidget->setCurrentWidget(mPageList->value("aboutTab"));
        return;
    }

    AboutPageWidget *aboutTab = new AboutPageWidget();
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

    SettingsPageWidget *settingsTab = new SettingsPageWidget();
    settingsTab->setObjectName(QString::fromUtf8("settingsTab"));

    mPageList->insert(settingsTab->objectName(), settingsTab);
    mTypePageList->push_back(PageType::SettingsPage);

    ui->tabWidget->addTab(settingsTab, QString::fromUtf8("Настройки"));
    ui->tabWidget->setCurrentWidget(settingsTab);
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

void MainWindow::tabCurrentChanged(int index)
{
    PageType::Enum pageType = mTypePageList->at(ui->tabWidget->currentIndex());

    ui->mainToolBar->setEnabled(pageType == PageType::DeviceListPage);
    ui->dslamToolBar->setEnabled(pageType == PageType::DslamPage);
    ui->switchToolBar->setEnabled(pageType == PageType::SwitchPage);

    if (index != 0) {
        ui->menuDslam->setEnabled(false);
        ui->menuSwitch->setEnabled(false);
        ui->menuOlt->setEnabled(false);
    } else {
        DeviceListPageWidget *deviceListPage = qobject_cast<DeviceListPageWidget *>(mPageList->value("deviceListTab"));
        deviceListPage->clearSelection();
    }
}

void MainWindow::deviceViewActivatedItem(QModelIndex index)
{
    if (!index.isValid())
        return;

    DeviceListPageWidget *deviceListPage = qobject_cast<DeviceListPageWidget *>(mPageList->value("deviceListTab"));

    DeviceListModel *deviceListModel = deviceListPage->deviceListModel();
    index = deviceListPage->proxyModel()->mapToSource(index);

    QModelIndex deviceTypeIndex = deviceListModel->index(index.row(), 3);
    QString deviceTypeString = deviceListModel->data(deviceTypeIndex).toString();
    DeviceType::Enum deviceType = DeviceType::from(deviceTypeString);

    ui->menuDslam->setEnabled(deviceType == DeviceType::Dslam);
    ui->menuSwitch->setEnabled(deviceType == DeviceType::Switch);
    ui->menuOlt->setEnabled(deviceType == DeviceType::Olt);
}

void MainWindow::saveSwitchConfig()
{
    if (mTypePageList->at(ui->tabWidget->currentIndex()) != PageType::SwitchPage)
        return;

    SwitchPageWidget *switchPageWidget = qobject_cast<SwitchPageWidget *>(ui->tabWidget->currentWidget());

    switchPageWidget->saveSwitchConfig();
}

void MainWindow::upDslPort()
{
    if (mTypePageList->at(ui->tabWidget->currentIndex()) != PageType::DslamPage)
        return;

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
