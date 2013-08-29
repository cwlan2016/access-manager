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

    SwitchPortTableModel *portTableModel = new SwitchPortTableModel(switchInfo, this);

    portTableModel->updateInfoAllPort();

    SwitchPortTableDelegate *portTableDelegate = new SwitchPortTableDelegate(this);
    portTableDelegate->setDescriptionPortLength(mDevice->maxLengthPortDescription());

    ui->portListTableView->setModel(portTableModel);
    ui->portListTableView->setItemDelegate(portTableDelegate);
    ui->portListTableView->setColumnWidth(0, 50);
    ui->portListTableView->setColumnWidth(1, 70);
    ui->portListTableView->setColumnWidth(2, 145);

    connect(ui->portListTableView, &QTableView::customContextMenuRequested,
            this, &SwitchPageWidget::portViewRequestContextMenu);

    //Инициализация модели таблицы mac-адресов
    MacTableModel *macTableModel = new MacTableModel(switchInfo, this);
    connect(macTableModel, &MacTableModel::updateFinished,
            this, &SwitchPageWidget::updateMacTableFinished);

    macTableModel->update();

    QSortFilterProxyModel *macListFilterProxyModel = new QSortFilterProxyModel(this);
    macListFilterProxyModel->setFilterRole(Qt::DisplayRole);
    macListFilterProxyModel->setFilterKeyColumn(2);
    macListFilterProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    macListFilterProxyModel->setSourceModel(macTableModel);

    ui->macAddressTableView->setModel(macListFilterProxyModel);
    ui->macAddressTableView->setColumnWidth(0, 70);
    ui->macAddressTableView->setColumnWidth(1, 100);

    connect(ui->macAddressTableView, &QTableView::customContextMenuRequested,
            this, &SwitchPageWidget::macTableViewRequestContextMenu);
}

void SwitchPageWidget::saveSwitchConfig()
{
    bool result = mDevice.objectCast<Switch>()->saveConfig();

    if (result) {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Запрос на сохранение конфигурации коммутатора отправлен."));
    } else {
        BasicDialogs::information(this, BasicDialogStrings::Error, mDevice->error());
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
    SwitchPortTableModel *portListModel = static_cast<SwitchPortTableModel *>(ui->portListTableView->model());

    QModelIndex index = ui->portListTableView->currentIndex();
    QModelIndex portIndex = portListModel->index(index.row(), 0);
    int port = portListModel->data(portIndex).toInt();

    if (!index.isValid())
        return;

    bool result = portListModel->updateInfoPort(port);

    if (!result) {
        BasicDialogs::information(this, BasicDialogStrings::Error, portListModel->error());
    } else {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Информация по порту %1 обновлена.").arg(port));

        if (ui->portInfoFrame->isVisible())
            showPortInfoFrame();
    }
}

void SwitchPageWidget::refreshAllPortInfo()
{
    SwitchPortTableModel *portListModel = static_cast<SwitchPortTableModel *>(ui->portListTableView->model());

    ui->portInfoFrame->setVisible(false);

    if (!portListModel->updateInfoAllPort()) {
        BasicDialogs::error(this, BasicDialogStrings::Error, portListModel->error());
    } else {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Информация по всем портам обновлена."));
    }
}

void SwitchPageWidget::refreshMacTable()
{
    QSortFilterProxyModel *proxyModel = static_cast<QSortFilterProxyModel *>(ui->macAddressTableView->model());
    MacTableModel *macListModel = static_cast<MacTableModel *>(proxyModel->sourceModel());

    macListModel->update();
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
    SwitchPortTableModel *portListModel = static_cast<SwitchPortTableModel *>(ui->portListTableView->model());

    QModelIndex index = ui->portListTableView->currentIndex();
    QModelIndex portIndex = portListModel->index(index.row(), 0);
    int port = portListModel->data(portIndex).toInt();

    if (!index.isValid())
        return;

    if (port == 25 || port == 26) {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Настройки на данном порту менять нельзя."));
        return;
    }

    bool result = portListModel->setMemberMulticastVlan(port, state);

    if (!result) {
        BasicDialogs::error(this, BasicDialogStrings::Error, portListModel->error());
    } else {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Настойки порта изменены."));

        if (ui->portInfoFrame->isVisible())
            showPortInfoFrame();
    }
}

void SwitchPageWidget::setPortInternetService()
{
    SwitchPortTableModel *portListModel = static_cast<SwitchPortTableModel *>(ui->portListTableView->model());

    QModelIndex index = ui->portListTableView->currentIndex();
    QModelIndex portIndex = portListModel->index(index.row(), 0);
    int port = portListModel->data(portIndex).toInt();

    if (!index.isValid())
        return;

    if (port == 25 || port == 26) {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Настройки на данном порту менять нельзя."));
        return;
    }

    bool result = portListModel->setMemberInternetService(port);

    if (!result) {
        BasicDialogs::error(this, BasicDialogStrings::Error, portListModel->error());
    } else {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Порт %1 настроен для Интернета.").arg(port));

        if (ui->portInfoFrame->isVisible())
            showPortInfoFrame();
    }
}

void SwitchPageWidget::setPortInternetWithStbService()
{
    SwitchPortTableModel *portListModel = static_cast<SwitchPortTableModel *>(ui->portListTableView->model());

    QModelIndex index = ui->portListTableView->currentIndex();
    QModelIndex portIndex = portListModel->index(index.row(), 0);
    int port = portListModel->data(portIndex).toInt();

    if (!index.isValid())
        return;

    if (port == 25 || port == 26) {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Настройки на данном порту менять нельзя."));
        return;
    }

    bool result = portListModel->setMemberInternetWithIptvStbService(port);

    if (!result) {
        BasicDialogs::error(this, BasicDialogStrings::Error, portListModel->error());
    } else {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Порт настроен для Интернета + Iptv Stb"));

        if (ui->portInfoFrame->isVisible())
            showPortInfoFrame();
    }
}

void SwitchPageWidget::filterMacAddressByPorts()
{
    QSortFilterProxyModel *proxyModel = static_cast<QSortFilterProxyModel *>(ui->macAddressTableView->model());

    proxyModel->setFilterKeyColumn(0);

    if (ui->portListLineEdit->text().isEmpty()) {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Перед фильтрованием необходимо ввести список портов."));
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

    proxyModel->setFilterKeyColumn(2);
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
    SwitchPortTableModel *portListModel = static_cast<SwitchPortTableModel *>(ui->portListTableView->model());

    QModelIndex index = ui->portListTableView->currentIndex();

    if (!index.isValid())
        return;

    QModelIndex portIndex = portListModel->index(index.row(), 0);
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

void SwitchPageWidget::updateMacTableFinished(bool withErrors)
{
    if (withErrors) {
        QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel *>(ui->macAddressTableView->model());
        MacTableModel *model = qobject_cast<MacTableModel *>(proxyModel->sourceModel());
        BasicDialogs::error(this, BasicDialogStrings::Error,
                            QString::fromUtf8("Обновление таблицы MAC-адресов завершилось ошибкой."),
                            model->error());
    } else {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Обновление таблицы MAC-адресов завершилось успешно."));
    }
}
