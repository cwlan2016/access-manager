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
    connect(ui->updateServiceDataAction, &QAction::triggered,
            this, &MainWindow::getServiceDataFromDevice);

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
    deviceListPage->init();
    deviceListPage->setObjectName(QString::fromUtf8("deviceListTab"));

    connect(ui->loadDeviceListAction, &QAction::triggered,
            deviceListPage, &DeviceTablePageWidget::loadDeviceList);
    connect(ui->saveDeviceListAction, &QAction::triggered,
            deviceListPage, &DeviceTablePageWidget::saveDeviceList);
    connect(ui->updateVlanAllSwitchAction, &QAction::triggered,
            deviceListPage, &DeviceTablePageWidget::batchUpdateVlansSwitch);
    connect(ui->updateAllDslamBoardsInfoAction, &QAction::triggered,
            deviceListPage, &DeviceTablePageWidget::batchUpdateBoardsDslam);
    connect(ui->updateInfoAllDevicesAction, &QAction::triggered,
            deviceListPage,  &DeviceTablePageWidget::batchUpdateInfoAllDevices);
    connect(ui->updateAllProfileOltInfoAction, &QAction::triggered,
            deviceListPage, &DeviceTablePageWidget::batchUpdateProfilesOlt);

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
        DevicePageWidget *widget = qobject_cast<DevicePageWidget *>(ui->tabWidget->currentWidget());

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

    QPixmap pixmap(":/images/configure.png");
    ui->tabWidget->addTab(settingsTab, pixmap, QString::fromUtf8("Настройки"));
    ui->tabWidget->setCurrentWidget(settingsTab);
}

void MainWindow::tabCurrentChanged(int index)
{
    PageType::Enum pageType = mTypePageList->at(ui->tabWidget->currentIndex());

    if (index == 0) {
        DeviceTablePageWidget *deviceListPage = qobject_cast<DeviceTablePageWidget *>(mPageList->value("deviceListTab"));
        deviceListPage->clearSelection();
    }

    if ((pageType == PageType::DeviceListPage)
            || (pageType == PageType::DslamPage)
            || (pageType == PageType::SwitchPage)
            || (pageType == PageType::OltPage)) {
        ui->updateServiceDataAction->setEnabled(true);

        PageWidget *page = qobject_cast<PageWidget *>(ui->tabWidget->currentWidget());
        ui->currentTabAction->setMenu(page->contextMenu());
    } else {
        ui->updateServiceDataAction->setEnabled(false);
        ui->currentTabAction->setMenu(new QMenu(this));
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
        return;
    } else if (result == QMessageBox::YesRole) {
        if (!deviceListModel->save()) {
            BasicDialogs::warning(this, QString::fromUtf8("Сохранение данных"), QString::fromUtf8("Ошибка: сохранение списка устройств не удалось."), deviceListModel->error());
        } else {
            BasicDialogs::information(this, QString::fromUtf8("Сохранение данных"), QString::fromUtf8("Информация: список устройств сохранен."));
        }
    }

    event->accept();
    DslamProfileConfig::deinit();
}
