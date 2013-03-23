#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init_snmp("snmpapp");
    SOCK_STARTUP;

    //Menu Actions
    connect(ui->exitAction, SIGNAL(triggered()), SLOT(close()));
    connect(ui->aboutAction, SIGNAL(triggered()), SLOT(showAboutProgramPage()));
    connect(ui->settingsAction, SIGNAL(triggered()), SLOT(showSettingsPage()));
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), SLOT(tabCloseRequested(int)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), SLOT(tabCurrentChanged(int)));
    connect(ui->updateVlanAllSwitchAction, SIGNAL(triggered()), SLOT(sUpdateVlanInfoAllSwitch()));
    connect(ui->updateAllDslamBoardsInfoAction, SIGNAL(triggered()), SLOT(sUpdateBoardInfoAllDslam()));
    connect(ui->editDslamBoardListAction, SIGNAL(triggered()), SLOT(showEditDslamBoardListPage()));
    connect(ui->saveConfigSwitchAction, SIGNAL(triggered()), SLOT(saveSwitchConfig()));
    connect(ui->upPortAction, SIGNAL(triggered()), SLOT(upDslPort()));
    connect(ui->downPortAction, SIGNAL(triggered()), SLOT(downDslPort()));
    //Panels actions
    connect(ui->mainBarAction, SIGNAL(triggered(bool)), ui->mainToolBar, SLOT(setVisible(bool)));
    connect(ui->mainToolBar, SIGNAL(visibilityChanged(bool)), ui->mainBarAction, SLOT(setChecked(bool)));
    connect(ui->switchBarAction, SIGNAL(triggered(bool)), ui->switchToolBar, SLOT(setVisible(bool)));
    connect(ui->switchToolBar, SIGNAL(visibilityChanged(bool)), ui->switchBarAction, SLOT(setChecked(bool)));
    connect(ui->dslamBarAction, SIGNAL(triggered(bool)), ui->dslamToolBar, SLOT(setVisible(bool)));
    connect(ui->dslamToolBar, SIGNAL(visibilityChanged(bool)), ui->dslamBarAction, SLOT(setChecked(bool)));

    ui->menuSwitch->setEnabled(false);
    ui->menuDslam->setEnabled(false);

    this->setWindowState(Qt::WindowMaximized);

    mTypePageList = new QVector<PageType>();
    mPageList = new QHash<QString, QWidget*>();
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

    connect(ui->addDeviceAction, SIGNAL(triggered()), deviceListPage, SLOT(addDevice()));
    connect(ui->editDeviceAction, SIGNAL(triggered()), deviceListPage, SLOT(editDevice()));
    connect(ui->removeDeviceAction, SIGNAL(triggered()), deviceListPage, SLOT(removeDevice()));
    connect(ui->loadDeviceListAction, SIGNAL(triggered()), deviceListPage, SLOT(loadDeviceList()));
    connect(ui->saveDeviceListAction, SIGNAL(triggered()), deviceListPage, SLOT(saveDeviceList()));
    connect(ui->updateVlanSwitchAction, SIGNAL(triggered()), deviceListPage, SLOT(updateVlanInfoSwitch()));
    connect(ui->updateDslamBoardsInfoAction, SIGNAL(triggered()), deviceListPage, SLOT(updateBoardInfoDslam()));
    connect(ui->updateVlanAllSwitchAction, SIGNAL(triggered()), deviceListPage, SLOT(batchUpdateVlanInfoSwitch()));
    connect(ui->updateAllDslamBoardsInfoAction, SIGNAL(triggered()), deviceListPage, SLOT(batchUpdateBoardsInfoDslam()));
    connect(ui->updateInfoAllDevicesAction, SIGNAL(triggered()), deviceListPage, SLOT(batchUpdateInfoAllDevices()));
    connect(deviceListPage, SIGNAL(changedActiveItem(QModelIndex)), SLOT(deviceViewActivatedItem(QModelIndex)));

    mPageList->insert(deviceListPage->objectName(), deviceListPage);
    mTypePageList->append(PageType::DeviceListPage);

    ui->tabWidget->addTab(deviceListPage, QString::fromUtf8("Оборудование"));
    ui->tabWidget->setCurrentWidget(deviceListPage);
}

void MainWindow::loadDeviceList()
{
    ui->loadDeviceListAction->trigger();
}

void MainWindow::loadProgramSettings()
{
    if (!Config::load())
    {
        BasicDialogs::error(this, BasicDialogTitle::Error, Config::errorString());
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    DeviceListPageWidget *deviceListPage = qobject_cast<DeviceListPageWidget*>(mPageList->value("deviceListTab"));
    DeviceListModel *deviceListModel = deviceListPage->deviceListModel();

    if (!deviceListModel->isModified())
    {
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

    if (result == QMessageBox::RejectRole)
    {
        event->ignore();
    }
    else if (result == QMessageBox::YesRole)
    {
        if (!deviceListModel->save())
        {
            BasicDialogs::warning(this, QString::fromUtf8("Сохранение данных"), QString::fromUtf8("Ошибка: сохранение списка устройств не удалось."), deviceListModel->error());
        }

        event->accept();
    }
}

void MainWindow::showAboutProgramPage()
{
    if (mPageList->contains("aboutTab"))
    {
        ui->tabWidget->setCurrentWidget(mPageList->value("aboutTab"));
        return;
    }

    AboutPageWidget* aboutTab = new AboutPageWidget();
    aboutTab->setObjectName(QString::fromUtf8("aboutTab"));

    mPageList->insert(aboutTab->objectName(), aboutTab);
    mTypePageList->append(PageType::AboutPage);

    ui->tabWidget->addTab(aboutTab, QString::fromUtf8("О программе"));
    ui->tabWidget->setCurrentWidget(aboutTab);
}

void MainWindow::showSettingsPage()
{
    if (mPageList->contains("settingsTab"))
    {
        ui->tabWidget->setCurrentWidget(mPageList->value("settingsTab"));
        return;
    }

    SettingsPageWidget* settingsTab = new SettingsPageWidget();
    settingsTab->setObjectName(QString::fromUtf8("settingsTab"));

    mPageList->insert(settingsTab->objectName(), settingsTab);
    mTypePageList->append(PageType::SettingsPage);

    ui->tabWidget->addTab(settingsTab, QString::fromUtf8("Настройки"));
    ui->tabWidget->setCurrentWidget(settingsTab);
}

void MainWindow::tabCloseRequested(int index)
{
    if (index != 0)
    {
        QWidget* deletedWidget = ui->tabWidget->widget(index);
        ui->tabWidget->removeTab(index);
        mPageList->remove(deletedWidget->objectName());
        mTypePageList->remove(index);

        delete deletedWidget;
    }
}

void MainWindow::tabCurrentChanged(int index)
{
    PageType pageType = mTypePageList->at(ui->tabWidget->currentIndex());

    ui->mainToolBar->setEnabled(pageType == PageType::DeviceListPage);
    ui->dslamToolBar->setEnabled(pageType == PageType::DslamPage);
    ui->switchToolBar->setEnabled(pageType == PageType::SwitchPage);

    if (index != 0)
    {
        ui->menuDslam->setEnabled(false);
        ui->menuSwitch->setEnabled(false);
    }
    else
    {
        DeviceListPageWidget *deviceListPage = qobject_cast<DeviceListPageWidget*>(mPageList->value("deviceListTab"));
        deviceListPage->clearSelection();
    }
}

void MainWindow::deviceViewActivatedItem(QModelIndex index)
{
    if (!index.isValid())
        return;

    DeviceListPageWidget *deviceListPage = qobject_cast<DeviceListPageWidget*>(mPageList->value("deviceListTab"));

    DeviceListModel *deviceListModel = deviceListPage->deviceListModel();
    index = deviceListPage->proxyModel()->mapToSource(index);

    DeviceType deviceType = DeviceTypeFromString(deviceListModel->data(deviceListModel->index(index.row(), 3)).toString());

    ui->menuDslam->setEnabled(deviceType == DeviceType::Dslam);
    ui->menuSwitch->setEnabled(deviceType == DeviceType::Switch);
}

void MainWindow::saveSwitchConfig()
{
    if (mTypePageList->at(ui->tabWidget->currentIndex()) != PageType::SwitchPage)
        return;

    SwitchPageWidget* switchPageWidget = qobject_cast<SwitchPageWidget*>(ui->tabWidget->currentWidget());

    switchPageWidget->saveSwitchConfig();
}

void MainWindow::upDslPort()
{
    if (mTypePageList->at(ui->tabWidget->currentIndex()) != PageType::DslamPage)
        return;

    DslamPageWidget* dslamPageWidget = qobject_cast<DslamPageWidget*>(ui->tabWidget->currentWidget());

    dslamPageWidget->upDslPort();
}

void MainWindow::downDslPort()
{
    if (mTypePageList->at(ui->tabWidget->currentIndex()) != PageType::DslamPage)
        return;

    DslamPageWidget* dslamPageWidget = qobject_cast<DslamPageWidget*>(ui->tabWidget->currentWidget());

    dslamPageWidget->downDslPort();
}
