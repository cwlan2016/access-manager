#include "switchpagewidget.h"
#include "ui_switchpagewidget.h"

#include <basicdialogs.h>
#include <constant.h>
#include <devices/switch.h>
#include <models/mactablemodel.h>
#include <models/switchporttabledelegate.h>
#include <models/switchporttablemodel.h>

SwitchPageWidget::SwitchPageWidget(Device::Ptr deviceInfo, QWidget *parent) :
    DevicePageWidget(deviceInfo, parent),
    ui(new Ui::SwitchPageWidget)
{
    ui->setupUi(this);

    ui->messageWidget->hide();
    ui->messageWidget->setCloseButtonVisible(true);
    ui->macModelMessage->hide();
    ui->macModelMessage->setCloseButtonVisible(true);
}

SwitchPageWidget::~SwitchPageWidget()
{
    delete ui;
}

void SwitchPageWidget::init()
{
    initActions();
    initComponents();
    initMenu();
    initView();
}

void SwitchPageWidget::showMessage(const QString &msg, ImprovedMessageWidget::MessageType messageType,
                                   const QString &detailedText)
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

void SwitchPageWidget::initActions()
{
    connect(ui->showPortInfoAction, &QAction::triggered,
            this, &SwitchPageWidget::showPortInfoFrame);
    connect(ui->editPortDescAction, &QAction::triggered,
            this, &SwitchPageWidget::editDescPort);
    connect(ui->refreshPortInfoAction, &QAction::triggered,
            this, &SwitchPageWidget::refreshPortInfo);
    connect(ui->refreshAllPortInfoAction, &QAction::triggered,
            this, &SwitchPageWidget::refreshAllPortInfo);
    connect(ui->refreshMacTableInfoAction, &QAction::triggered,
            this, &SwitchPageWidget::refreshMacTable);
    connect(ui->addPortToMulticastVlanAction, &QAction::triggered,
            this, &SwitchPageWidget::addPortToMulticastVlan);
    connect(ui->removePortFromMulticastVlanAction, &QAction::triggered,
            this, &SwitchPageWidget::removePortFromMulticastVlan);
    connect(ui->setPortInetServiceAction, &QAction::triggered,
            this, &SwitchPageWidget::setPortInternetService);
    connect(ui->setPortInetWithIptvStbServiceAction, &QAction::triggered,
            this, &SwitchPageWidget::setPortInternetWithStbService);
    connect(ui->saveConfigAction, &QAction::triggered,
            this, &SwitchPageWidget::saveSwitchConfig);

    ui->saveConfigButton->setDefaultAction(ui->saveConfigAction);
    ui->editPortDescButton->setDefaultAction(ui->editPortDescAction);
    ui->refreshAllPortInfoButton->setDefaultAction(ui->refreshAllPortInfoAction);
    ui->refreshMacTableInfoButton->setDefaultAction(ui->refreshMacTableInfoAction);
}

void SwitchPageWidget::initComponents()
{
    ui->portInfoFrame->setVisible(false);

    connect(ui->closePortInfoButton, &QToolButton::pressed,
            ui->portInfoFrame, &QFrame::hide);
    connect(ui->macRadioButton, &QRadioButton::toggled,
            this, &SwitchPageWidget::macRadioButtonChangeState);
    connect(ui->macLineEdit, &QLineEdit::textChanged,
            this, &SwitchPageWidget::macLineEditTextChanged);
    connect(ui->filterPortButton, &QPushButton::pressed,
            this, &SwitchPageWidget::filterMacAddressByPorts);

    Switch::Ptr switchInfo = qobject_cast<Switch::Ptr>(mDevice);

    ui->portListLineEdit->setText(QString("1-%1").arg(switchInfo->countPorts()));
}

void SwitchPageWidget::initMenu()
{
    mContextMenu->addAction(ui->showPortInfoAction);
    mContextMenu->addAction(ui->refreshPortInfoAction);
    mContextMenu->addAction(ui->editPortDescAction);
    mContextMenu->addSeparator();

    QMenu *mvlanMenu = mContextMenu->addMenu(QIcon(":/images/tv.png"), "Multicast Vlan");
    mvlanMenu->addAction(ui->addPortToMulticastVlanAction);
    mvlanMenu->addAction(ui->removePortFromMulticastVlanAction);

    QMenu *settingsPortMenu = mContextMenu->addMenu(QString::fromUtf8("Установить сервис"));
    settingsPortMenu->addAction(ui->setPortInetServiceAction);
    settingsPortMenu->addAction(ui->setPortInetWithIptvStbServiceAction);

    mContextMenu->addSeparator();
    mContextMenu->addAction(ui->refreshAllPortInfoAction);

    mContextMenu->addSeparator();
    mContextMenu->addAction(ui->refreshMacTableInfoAction);
}

void SwitchPageWidget::initView()
{
    //Инициализация модели списка портов
    Switch::Ptr switchInfo = mDevice.objectCast<Switch>();

    SwitchPortTableModel *portTableModel = new SwitchPortTableModel(switchInfo,
                                                                    ui->messageWidget, this);
    connect(portTableModel, &SwitchPortTableModel::updateFinished,
            this, &SwitchPageWidget::updatePortTableFinished);

    portTableModel->update();

    QSortFilterProxyModel *portProxyModel = new QSortFilterProxyModel(this);
    portProxyModel->setSourceModel(portTableModel);

    SwitchPortTableDelegate *portTableDelegate = new SwitchPortTableDelegate(this);
    portTableDelegate->setDescriptionPortLength(mDevice->maxLengthPortDescription());

    ui->portListTableView->setModel(portProxyModel);
    ui->portListTableView->setItemDelegate(portTableDelegate);
    ui->portListTableView->setColumnWidth(SwitchPortTableModel::NumberColumn, 50);
    ui->portListTableView->setColumnWidth(SwitchPortTableModel::StateColumn, 70);
    ui->portListTableView->setColumnWidth(SwitchPortTableModel::SpeedDuplexColumn, 145);

    connect(ui->portListTableView, &QTableView::customContextMenuRequested,
            this, &SwitchPageWidget::portViewRequestContextMenu);

    //Инициализация модели таблицы mac-адресов
    MacTableModel *macTableModel = new MacTableModel(switchInfo, this);
    connect(macTableModel, &MacTableModel::updateFinished,
            this, &SwitchPageWidget::updateMacTableFinished);

    macTableModel->update();

    QSortFilterProxyModel *macListProxyModel = new QSortFilterProxyModel(this);
    macListProxyModel->setFilterRole(Qt::DisplayRole);
    macListProxyModel->setFilterKeyColumn(MacTableModel::MacAddressColumn);
    macListProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    macListProxyModel->setSourceModel(macTableModel);

    ui->macAddressTableView->setModel(macListProxyModel);
    ui->macAddressTableView->setColumnWidth(MacTableModel::PortColumn, 70);
    ui->macAddressTableView->setColumnWidth(MacTableModel::VlanColumn, 100);

    connect(ui->macAddressTableView, &QTableView::customContextMenuRequested,
            this, &SwitchPageWidget::macTableViewRequestContextMenu);
}

void SwitchPageWidget::saveSwitchConfig()
{
    bool result = mDevice.objectCast<Switch>()->saveConfig();

    if (result) {
        showMessage(trUtf8("Запрос на сохранение конфигурации коммутатора отправлен."));
    } else {
        showMessage(mDevice->error(), ImprovedMessageWidget::Error);
    }
}

void SwitchPageWidget::editDescPort()
{
    QModelIndex currentPort = ui->portListTableView->currentIndex();

    if (!currentPort.isValid())
        return;

    QModelIndex descIndex = ui->portListTableView->model()->index(currentPort.row(),
                                                              SwitchPortTableModel::DescColumn);
    ui->portListTableView->setFocus();
    ui->portListTableView->edit(descIndex);
}

void SwitchPageWidget::refreshPortInfo()
{
    QSortFilterProxyModel *proxyModel = static_cast<QSortFilterProxyModel *>(ui->portListTableView->model());
    SwitchPortTableModel *portListModel = static_cast<SwitchPortTableModel *>(proxyModel->sourceModel());

    QModelIndex index = proxyModel->mapToSource(ui->portListTableView->currentIndex());
    QModelIndex portIndex = portListModel->index(index.row(), SwitchPortTableModel::NumberColumn);
    int port = portListModel->data(portIndex).toInt();

    if (!index.isValid())
        return;

    if (portListModel->updateIsRunning()) {
        showMessage(trUtf8("Уже выполняется обновление информации по всем портам."));
        return;
    }

    bool result = portListModel->updatePort(port);

    if (!result) {
        showMessage(portListModel->error(), ImprovedMessageWidget::Error);
    } else {
        showMessage(trUtf8("Информация по порту %1 обновлена.").arg(port));

        if (ui->portInfoFrame->isVisible())
            showPortInfoFrame();
    }
}

void SwitchPageWidget::refreshAllPortInfo()
{
    QSortFilterProxyModel *proxyModel = static_cast<QSortFilterProxyModel *>(ui->portListTableView->model());
    SwitchPortTableModel *portListModel = static_cast<SwitchPortTableModel *>(proxyModel->sourceModel());

    ui->portInfoFrame->setVisible(false);

    if (portListModel->updateIsRunning()) {
        showMessage(trUtf8("Уже выполняется обновление информации по всем портам."));
    } else {
        portListModel->update();
    }
}

void SwitchPageWidget::refreshMacTable()
{
    if (ui->macModelMessage->isVisible())
        ui->macModelMessage->animatedHide();

    QSortFilterProxyModel *proxyModel = static_cast<QSortFilterProxyModel *>(ui->macAddressTableView->model());
    MacTableModel *macListModel = static_cast<MacTableModel *>(proxyModel->sourceModel());

    if (macListModel->updateIsRunning()) {
        showMessage(trUtf8("Обновление таблицы MAC-адресов уже выполняется."));
    } else {
        macListModel->update();
    }
}

void SwitchPageWidget::addPortToMulticastVlan()
{
    changeStateSwitchPortInMulticastVlan(true);
}

void SwitchPageWidget::removePortFromMulticastVlan()
{
    changeStateSwitchPortInMulticastVlan(false);
}

void SwitchPageWidget::changeStateSwitchPortInMulticastVlan(bool state)
{
    QSortFilterProxyModel *proxyModel = static_cast<QSortFilterProxyModel *>(ui->portListTableView->model());
    SwitchPortTableModel *portListModel = static_cast<SwitchPortTableModel *>(proxyModel->sourceModel());

    QModelIndex index = proxyModel->mapToSource(ui->portListTableView->currentIndex());
    QModelIndex portIndex = portListModel->index(index.row(), SwitchPortTableModel::NumberColumn);
    int port = portListModel->data(portIndex).toInt();

    if (!index.isValid())
        return;

    if (port == 25 || port == 26) {
        showMessage(trUtf8("Настройки на данном порту менять нельзя."));
        return;
    }

    bool result = portListModel->setMemberMulticastVlan(port, state);

    if (!result) {
        showMessage(portListModel->error(), ImprovedMessageWidget::Error);
    } else {
        showMessage(trUtf8("Настойки порта %1 изменены.").arg(port));

        if (ui->portInfoFrame->isVisible())
            showPortInfoFrame();
    }
}

void SwitchPageWidget::setPortInternetService()
{
    QSortFilterProxyModel *proxyModel = static_cast<QSortFilterProxyModel *>(ui->portListTableView->model());
    SwitchPortTableModel *portListModel = static_cast<SwitchPortTableModel *>(proxyModel->sourceModel());

    QModelIndex index = proxyModel->mapToSource(ui->portListTableView->currentIndex());
    QModelIndex portIndex = portListModel->index(index.row(), SwitchPortTableModel::NumberColumn);
    int port = portListModel->data(portIndex).toInt();

    if (!index.isValid())
        return;

    if (port == 25 || port == 26) {
        showMessage(trUtf8("Настройки на данном порту менять нельзя."));
        return;
    }

    bool result = portListModel->setMemberInternetService(port);

    if (!result) {
        showMessage(portListModel->error(), ImprovedMessageWidget::Error);
    } else {
        showMessage(trUtf8("Порт %1 настроен для Интернета.").arg(port));

        if (ui->portInfoFrame->isVisible())
            showPortInfoFrame();
    }
}

void SwitchPageWidget::setPortInternetWithStbService()
{
    QSortFilterProxyModel *proxyModel = static_cast<QSortFilterProxyModel *>(ui->portListTableView->model());
    SwitchPortTableModel *portListModel = static_cast<SwitchPortTableModel *>(proxyModel->sourceModel());

    QModelIndex index = proxyModel->mapToSource(ui->portListTableView->currentIndex());
    QModelIndex portIndex = portListModel->index(index.row(), SwitchPortTableModel::NumberColumn);
    int port = portListModel->data(portIndex).toInt();

    if (!index.isValid())
        return;

    if (port == 25 || port == 26) {
        showMessage(trUtf8("Настройки на данном порту менять нельзя."));
        return;
    }

    bool result = portListModel->setMemberInternetWithIptvStbService(port);

    if (!result) {
        showMessage(portListModel->error(), ImprovedMessageWidget::Error);
    } else {
        showMessage(trUtf8("Порт %1 настроен для Интернета + Iptv Stb").arg(port));

        if (ui->portInfoFrame->isVisible())
            showPortInfoFrame();
    }
}

void SwitchPageWidget::filterMacAddressByPorts()
{
    QSortFilterProxyModel *proxyModel = static_cast<QSortFilterProxyModel *>(ui->macAddressTableView->model());

    proxyModel->setFilterKeyColumn(MacTableModel::PortColumn);

    if (ui->portListLineEdit->text().isEmpty()) {
        showMessage(trUtf8("Перед фильтрованием необходимо ввести список портов."));
        return;
    }

    QString regExpStr = "";

    QStringList rangeStringList = ui->portListLineEdit->text().split(QRegExp(","), QString::SkipEmptyParts);

    auto it = rangeStringList.constBegin();
    auto end = rangeStringList.constEnd();
    for (; it != end; ++it) {
        QStringList valueStringList = (*it).split("-", QString::SkipEmptyParts);

        if (valueStringList.size() == 2) {
            int first = valueStringList.at(0).toInt();
            int last = valueStringList.at(1).toInt();

            for (int i = first; i <= last; ++i)
                regExpStr += QString("^%1$|").arg(i);
        } else {
            regExpStr += QString("^%1$|").arg(valueStringList.at(0));
        }
    }

    if (regExpStr[regExpStr.length() - 1] == '|')
        regExpStr.remove(regExpStr.length() - 1, 1);

    QRegExp regExp(regExpStr, Qt::CaseInsensitive, QRegExp::RegExp);
    proxyModel->setFilterRegExp(regExp);
}

void SwitchPageWidget::macLineEditTextChanged(QString text)
{
    QSortFilterProxyModel *proxyModel = static_cast<QSortFilterProxyModel *>(ui->macAddressTableView->model());

    text = ui->macLineEdit->displayText();
    text.replace('_', "?");

    proxyModel->setFilterKeyColumn(MacTableModel::MacAddressColumn);
    proxyModel->setFilterWildcard(text);
}

void SwitchPageWidget::macRadioButtonChangeState(bool checked)
{
    ui->macLineEdit->setEnabled(checked);
    ui->portListLineEdit->setEnabled(!checked);
    ui->filterPortButton->setEnabled(!checked);

    if (checked) {
        macLineEditTextChanged(ui->macLineEdit->text());
    } else {
        filterMacAddressByPorts();
    }
}

void SwitchPageWidget::showPortInfoFrame()
{
    QSortFilterProxyModel *proxyModel = static_cast<QSortFilterProxyModel *>(ui->portListTableView->model());
    SwitchPortTableModel *portListModel = static_cast<SwitchPortTableModel *>(proxyModel->sourceModel());

    QModelIndex index = proxyModel->mapToSource(ui->portListTableView->currentIndex());

    if (!index.isValid())
        return;

    QModelIndex portIndex = portListModel->index(index.row(), SwitchPortTableModel::NumberColumn);
    int port = portListModel->data(portIndex).toInt();
    ui->portInetVlanValueLabel->setText(VlanState::toString(portListModel->memberInetVlan(port)));
    ui->portIptvVlanValueLabel->setText(VlanState::toString(portListModel->memberIptvVlan(port)));

    if (portListModel->memberMulticastVlan(port)) {
        ui->multicastVlanValueLabel->setPixmap(QPixmap(QString::fromUtf8(":/images/yes.png")));
    } else {
        ui->multicastVlanValueLabel->setPixmap(QPixmap(QString::fromUtf8(":/images/no.png")));
    }

    ui->portInfoLabel->setText(QString::fromUtf8("Порт %1").arg(port));
    ui->portInfoFrame->setVisible(true);
}

void SwitchPageWidget::portViewRequestContextMenu(QPoint point)
{
    QMenu contextMenu(this);
    contextMenu.addAction(ui->showPortInfoAction);
    contextMenu.addAction(ui->refreshPortInfoAction);
    contextMenu.addAction(ui->editPortDescAction);
    contextMenu.addSeparator();

    QMenu *mvlanMenu = contextMenu.addMenu(QIcon(":/images/tv.png"), "Multicast Vlan");
    mvlanMenu->addAction(ui->addPortToMulticastVlanAction);
    mvlanMenu->addAction(ui->removePortFromMulticastVlanAction);

    QMenu *settingsPortMenu = contextMenu.addMenu(QString::fromUtf8("Установить сервис"));
    settingsPortMenu->addAction(ui->setPortInetServiceAction);
    settingsPortMenu->addAction(ui->setPortInetWithIptvStbServiceAction);

    contextMenu.addSeparator();
    contextMenu.addAction(ui->refreshAllPortInfoAction);

    contextMenu.exec(ui->portListTableView->mapToGlobal(point));
}

void SwitchPageWidget::macTableViewRequestContextMenu(QPoint point)
{
    QMenu contextMenu(this);
    contextMenu.addAction(ui->refreshMacTableInfoAction);

    contextMenu.exec(ui->macAddressTableView->mapToGlobal(point));
}

void SwitchPageWidget::updatePortTableFinished(bool withErrors)
{
    if (withErrors) {
        QSortFilterProxyModel *proxyModel = static_cast<QSortFilterProxyModel *>(ui->portListTableView->model());
        SwitchPortTableModel *portListModel = static_cast<SwitchPortTableModel *>(proxyModel->sourceModel());
        showMessage(trUtf8("Обновление информации по портам завершилось с ошибками."), ImprovedMessageWidget::Error, portListModel->error());
    } else {
        showMessage(trUtf8("Обновление информации по всем портам завершилось успешно."), ImprovedMessageWidget::Positive);
    }
}

void SwitchPageWidget::updateMacTableFinished(bool withErrors)
{
    QString message;

    if (withErrors) {
        message = trUtf8("Обновление таблицы MAC-адресов завершилось с ошибками.");
        QSortFilterProxyModel *proxyModel = static_cast<QSortFilterProxyModel *>(ui->macAddressTableView->model());
        MacTableModel *macTableModel = static_cast<MacTableModel *>(proxyModel->sourceModel());
        ui->macModelMessage->setMessageType(ImprovedMessageWidget::Error);
        ui->macModelMessage->setText(message, macTableModel->error());
    } else {
        message = trUtf8("Обновление таблицы MAC-адресов завершилось успешно.");
        ui->macModelMessage->setMessageType(ImprovedMessageWidget::Positive);
        ui->macModelMessage->setText(message);
        ui->macModelMessage->setWordWrap(true);
        ui->macModelMessage->updateGeometry();
    }

    ui->macModelMessage->animatedShow();
}
