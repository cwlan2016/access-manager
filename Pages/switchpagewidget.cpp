#include "switchpagewidget.h"
#include "ui_switchpagewidget.h"

#include <QtCore/QSortFilterProxyModel>
#include <QtWidgets/QMenu>
#ifdef _MSC_VER
#include "../basicdialogs.h"
#include "../constant.h"
#include "../Info/switchinfo.h"
#include "../Models/maclistmodel.h"
#include "../Models/switchportlistmodel.h"
#else
#include "basicdialogs.h"
#include "constant.h"
#include "Info/switchinfo.h"
#include "Models/maclistmodel.h"
#include "Models/switchportlistmodel.h"
#endif

SwitchPageWidget::SwitchPageWidget(DeviceInfo::Ptr deviceInfo, QWidget *parent) :
    PageWidget(deviceInfo, parent),
    ui(new Ui::SwitchPageWidget)
{
    ui->setupUi(this);

    ui->portInfoGroupBox->setVisible(false);

    connect(ui->portListTableView, &QTableView::customContextMenuRequested,
            this, &SwitchPageWidget::portViewRequestContextMenu);
    connect(ui->macAddressTableView, &QTableView::customContextMenuRequested,
            this, &SwitchPageWidget::macTableViewRequestContextMenu);
    connect(ui->portInfoGroupBox, &QGroupBox::toggled,
            ui->portInfoGroupBox, &QGroupBox::setVisible);

    connect(ui->macRadioButton, &QRadioButton::toggled,
            this, &SwitchPageWidget::macRadioButtonChangeState);
    connect(ui->macLineEdit, &QLineEdit::textChanged,
            this, &SwitchPageWidget::macLineEditTextChanged);
    connect(ui->filterPortButton, &QPushButton::pressed,
            this, &SwitchPageWidget::filterMacAddressByPorts);

    connect(ui->showPortInfoAction, &QAction::triggered,
            this, &SwitchPageWidget::showPortInfoGroupBox);
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

    //Инициализация модели списка портов
    SwitchInfo::Ptr info = std::static_pointer_cast<SwitchInfo>(mDeviceInfo);

    SwitchPortListModel *portListModel = new SwitchPortListModel(info->deviceModel(), this);
    portListModel->setIP(info->ip());

    portListModel->setInetVlanTag(info->inetVlanTag());
    portListModel->setIptvVlanTag(info->iptvVlanTag());
    portListModel->updateInfoAllPort();

    ui->portListTableView->setModel(portListModel);
    ui->portListTableView->setColumnWidth(0, 50);
    ui->portListTableView->setColumnWidth(1, 70);
    ui->portListTableView->setColumnWidth(2, 145);

    //Инициализация модели таблицы mac-адресов
    MacListModel *macListModel = new MacListModel(this);
    macListModel->setIP(info->ip());
    macListModel->setInetVlanTag(info->inetVlanTag());
    macListModel->setIptvVlanTag(info->iptvVlanTag());

    if (!macListModel->updateMacTable()) {
        BasicDialogs::error(this, BasicDialogTitle::Error, macListModel->error());
    }

    QSortFilterProxyModel *macListFilterProxyModel = new QSortFilterProxyModel(this);
    macListFilterProxyModel->setFilterRole(Qt::DisplayRole);
    macListFilterProxyModel->setFilterKeyColumn(2);
    macListFilterProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    macListFilterProxyModel->setSourceModel(macListModel);

    ui->macAddressTableView->setModel(macListFilterProxyModel);
    ui->macAddressTableView->setColumnWidth(0, 70);
    ui->macAddressTableView->setColumnWidth(1, 100);

    if (info->deviceModel() == DeviceModel::DES3550)
        ui->portListLineEdit->setText("1-50");
    else
        ui->portListLineEdit->setText("1-26");
}

SwitchPageWidget::~SwitchPageWidget()
{
    delete ui;
}

void SwitchPageWidget::changeStateSwitchPortInMulticastVlan(bool state)
{
    SwitchPortListModel *portListModel = static_cast<SwitchPortListModel *>(ui->portListTableView->model());

    QModelIndex index = ui->portListTableView->currentIndex();
    QModelIndex portIndex = portListModel->index(index.row(), 0);
    int port = portListModel->data(portIndex).toInt();

    if (!index.isValid())
        return;

    if (port == 25 || port == 26) {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Настройки на данном порту менять нельзя."));
        return;
    }

    bool result = portListModel->setMemberMulticastVlan(port, state);

    if (!result) {
        BasicDialogs::error(this, BasicDialogTitle::Error, portListModel->error());
    } else {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Настойки порта изменены."));

        if (ui->portInfoGroupBox->isVisible()) {
            showPortInfoGroupBox();
        }
    }
}

void SwitchPageWidget::saveSwitchConfig()
{
    bool result = std::static_pointer_cast<SwitchInfo>(mDeviceInfo)->saveConfig();

    if (result)
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Запрос на сохранение конфигурации коммутатора отправлен."));
    else
        BasicDialogs::information(this, BasicDialogTitle::Error, mDeviceInfo->error());
}

void SwitchPageWidget::showPortInfoGroupBox()
{
    SwitchPortListModel *portListModel = static_cast<SwitchPortListModel *>(ui->portListTableView->model());

    QModelIndex index = ui->portListTableView->currentIndex();

    if (!index.isValid())
        return;

    QModelIndex portIndex = portListModel->index(index.row(), 0);
    int port = portListModel->data(portIndex).toInt();
    ui->portInetVlanValueLabel->setText(VlanStateName[(short)portListModel->memberInetVlan(port)]);
    ui->portIptvVlanValueLabel->setText(VlanStateName[(short)portListModel->memberIptvVlan(port)]);

    if (portListModel->memberMulticastVlan(port)) {
        ui->multicastVlanValueLabel->setPixmap(QPixmap(QString::fromUtf8(":/images/yes.png")));
    } else {
        ui->multicastVlanValueLabel->setPixmap(QPixmap(QString::fromUtf8(":/images/no.png")));
    }

    ui->portInfoGroupBox->setTitle(QString::fromUtf8("Порт %1").arg(port));
    ui->portInfoGroupBox->setChecked(true);
}

void SwitchPageWidget::refreshPortInfo()
{
    SwitchPortListModel *portListModel = static_cast<SwitchPortListModel *>(ui->portListTableView->model());

    QModelIndex index = ui->portListTableView->currentIndex();
    QModelIndex portIndex = portListModel->index(index.row(), 0);
    int port = portListModel->data(portIndex).toInt();

    if (!index.isValid())
        return;

    bool result = portListModel->updateInfoPort(port);

    if (!result) {
        BasicDialogs::information(this, BasicDialogTitle::Error, portListModel->error());
    } else {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Информация по порту %1 обновлена.").arg(port));

        if (ui->portInfoGroupBox->isVisible()) {
            showPortInfoGroupBox();
        }
    }
}

void SwitchPageWidget::refreshAllPortInfo()
{
    SwitchPortListModel *portListModel = static_cast<SwitchPortListModel *>(ui->portListTableView->model());

    ui->portInfoGroupBox->setChecked(false);

    if (!portListModel->updateInfoAllPort()) {
        BasicDialogs::error(this, BasicDialogTitle::Error, portListModel->error());
    } else {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Информация по всем портам обновлена."));
    }
}

void SwitchPageWidget::refreshMacTable()
{
    QSortFilterProxyModel *proxyModel = static_cast<QSortFilterProxyModel *>(ui->macAddressTableView->model());
    MacListModel *macListModel = static_cast<MacListModel *>(proxyModel->sourceModel());

    if (!macListModel->updateMacTable()) {
        BasicDialogs::error(this, BasicDialogTitle::Error, macListModel->error());
    } else {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Информация в таблице MAC-адресов обновлена."));
    }
}

void SwitchPageWidget::portViewRequestContextMenu(QPoint point)
{
    QMenu contextMenu(this);
    contextMenu.addAction(ui->showPortInfoAction);
    contextMenu.addAction(ui->refreshPortInfoAction);
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

void SwitchPageWidget::addPortToMulticastVlan()
{
    changeStateSwitchPortInMulticastVlan(true);
}

void SwitchPageWidget::removePortFromMulticastVlan()
{
    changeStateSwitchPortInMulticastVlan(false);
}

void SwitchPageWidget::setPortInternetService()
{
    SwitchPortListModel *portListModel = static_cast<SwitchPortListModel *>(ui->portListTableView->model());

    QModelIndex index = ui->portListTableView->currentIndex();
    QModelIndex portIndex = portListModel->index(index.row(), 0);
    int port = portListModel->data(portIndex).toInt();

    if (!index.isValid())
        return;

    if (port == 25 || port == 26) {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Настройки на данном порту менять нельзя."));
        return;
    }

    bool result = portListModel->setMemberInternetService(port);

    if (!result) {
        BasicDialogs::error(this, BasicDialogTitle::Error, portListModel->error());
    } else {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Порт %1 настроен для Интернета.").arg(port));

        if (ui->portInfoGroupBox->isVisible()) {
            showPortInfoGroupBox();
        }
    }
}

void SwitchPageWidget::setPortInternetWithStbService()
{
    SwitchPortListModel *portListModel = static_cast<SwitchPortListModel *>(ui->portListTableView->model());

    QModelIndex index = ui->portListTableView->currentIndex();
    QModelIndex portIndex = portListModel->index(index.row(), 0);
    int port = portListModel->data(portIndex).toInt();

    if (!index.isValid())
        return;

    if (port == 25 || port == 26) {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Настройки на данном порту менять нельзя."));
        return;
    }

    bool result = portListModel->setMemberInternetWithIptvStbService(port);

    if (!result) {
        BasicDialogs::error(this, BasicDialogTitle::Error, portListModel->error());
    } else {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Порт настроен для Интернета + Iptv Stb"));

        if (ui->portInfoGroupBox->isVisible()) {
            showPortInfoGroupBox();
        }
    }
}

void SwitchPageWidget::filterMacAddressByPorts()
{
    QSortFilterProxyModel *proxyModel = static_cast<QSortFilterProxyModel *>(ui->macAddressTableView->model());

    proxyModel->setFilterKeyColumn(0);

    if (ui->portListLineEdit->text().isEmpty()) {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Перед фильтрованием необходимо ввести список портов."));
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

            for (int i = first; i <= last; ++i) {
                regExpStr += QString("^%1$|").arg(i);
            }
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

    proxyModel->setFilterKeyColumn(2);

    for (int i = 0; i < text.length() - 1; ++i) {
        if ((text[i] == '-') && (text[i + 1] == '-')) {
            if (i == 0) {
                text = "[0-9A-F]";
                break;
            } else {
                text.truncate(i);
                text.prepend('^');
                break;
            }
        }
    }

    QRegExp regExp(text, Qt::CaseInsensitive, QRegExp::RegExp);
    proxyModel->setFilterRegExp(regExp);
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
