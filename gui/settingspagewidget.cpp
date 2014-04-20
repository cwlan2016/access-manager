#include "settingspagewidget.h"
#include "ui_settingspagewidget.h"

#include <basicdialogs.h>
#include <config.h>
#include <constant.h>
#include <models/dslprofiletablemodel.h>

SettingsPageWidget::SettingsPageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsPageWidget)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);
    ui->listWidget->setCurrentRow(0);
    ui->deviceTabWidget->setCurrentIndex(0);

    ui->readComEdit->setValidator(new QRegExpValidator(QRegExp("\\w+"), this));
    ui->writeComEdit->setValidator(new QRegExpValidator(QRegExp("\\w+"), this));
    ui->timeoutEdit->setValidator(new QIntValidator(0, 100000, this));
    ui->saveTimeoutEdit->setValidator(new QIntValidator(0, 100000, this));
    ui->portEdit->setValidator(new QIntValidator(0, 65523, this));
    ui->retriesEdit->setValidator(new QIntValidator(0, 10, this));

    ui->messageWidget->setCloseButtonVisible(true);
    ui->messageWidget->hide();

    initGlobalActions();
    initSnmpTabFields();
    initDevicesTabFields();
    initDevicesTabActions();
}

SettingsPageWidget::~SettingsPageWidget()
{
    delete ui;
}

void SettingsPageWidget::showMessage(const QString &msg, ImprovedMessageWidget::MessageType messageType)
{
    if (msg.isEmpty())
        return;

    ui->messageWidget->setText(msg);
    ui->messageWidget->setMessageType(messageType);

    ui->messageWidget->setWordWrap(false);
    const int unwrappedWidth = ui->messageWidget->sizeHint().width();
    ui->messageWidget->setWordWrap(unwrappedWidth > size().width());

    ui->messageWidget->animatedShow();
}

void SettingsPageWidget::initGlobalActions()
{
    connect(ui->saveSettingsButton, &QPushButton::pressed,
            this, &SettingsPageWidget::saveSetting);
    connect(ui->defaultSettingButton, &QPushButton::pressed,
            this, &SettingsPageWidget::resetToDefault);
    connect(ui->listWidget, &QListWidget::currentRowChanged,
            this, &SettingsPageWidget::currentItemChanged);
}

void SettingsPageWidget::initSnmpTabFields()
{
    ui->readComEdit->setText(SnmpConfig::readCommunity());
    ui->writeComEdit->setText(SnmpConfig::writeCommunity());
    ui->timeoutEdit->setText(QString::number(SnmpConfig::timeout()));
    ui->saveTimeoutEdit->setText(QString::number(SnmpConfig::saveConfigTimeout()));
    ui->portEdit->setText(QString::number(SnmpConfig::port()));
    ui->retriesEdit->setText(QString::number(SnmpConfig::retries()));
}

void SettingsPageWidget::initDevicesTabFields()
{
    ui->swInternetVlanEdit->setText(SwitchConfig::inetVlanName());
    ui->swIptvUnicastVlanEdit->setText(SwitchConfig::iptvVlanName());

    ui->dsDeviceModelComboBox->setCurrentIndex(-1);
    ui->dsTypeBoardComboBox->setCurrentIndex(-1);
}

void SettingsPageWidget::initDevicesTabActions()
{
    ui->dsDslProfileTableView->addAction(ui->addDslProfileAction);
    ui->dsDslProfileTableView->addAction(ui->editDslProfileAction);
    ui->dsDslProfileTableView->addAction(ui->removeDslProfileAction);

    ui->dsAddProfileButton->setDefaultAction(ui->addDslProfileAction);
    ui->dsEditProfileButton->setDefaultAction(ui->editDslProfileAction);
    ui->dsRemoveProfileButton->setDefaultAction(ui->removeDslProfileAction);

    connect(ui->dsDeviceModelComboBox, static_cast<void (QComboBox:: *)(int)>(&QComboBox::currentIndexChanged),
            this, &SettingsPageWidget::dsComboBoxCurrentIndexChanged);
    connect(ui->dsTypeBoardComboBox, static_cast<void (QComboBox:: *)(int)>(&QComboBox::currentIndexChanged),
            this, &SettingsPageWidget::dsComboBoxCurrentIndexChanged);
    connect(ui->addDslProfileAction, &QAction::triggered,
            this, &SettingsPageWidget::addDslProfile);
    connect(ui->editDslProfileAction, &QAction::triggered,
            this, &SettingsPageWidget::editDslProfile);
    connect(ui->removeDslProfileAction, &QAction::triggered,
            this, &SettingsPageWidget::removeDslProfile);
}

void SettingsPageWidget::saveSetting()
{
    if (!validateSettingsData())
        return;

    SnmpConfig::setReadCommunity(ui->readComEdit->text());
    SnmpConfig::setWriteCommunity(ui->writeComEdit->text());
    SnmpConfig::setTimeout(ui->timeoutEdit->text().toUInt());
    SnmpConfig::setSaveConfigTimeout(ui->saveTimeoutEdit->text().toUInt());
    SnmpConfig::setPort(ui->portEdit->text().toUInt());
    SnmpConfig::setRetries(ui->retriesEdit->text().toUInt());

    SwitchConfig::setInetVlanName(ui->swInternetVlanEdit->text().trimmed());
    SwitchConfig::setIptvVlanName(ui->swIptvUnicastVlanEdit->text().trimmed());

    if (!Config::save()) {
        showMessage(Config::error(), ImprovedMessageWidget::Error);
        return;
    } else {
        showMessage(trUtf8("Настройки программы сохранены."), ImprovedMessageWidget::Positive);
        return;
    }
}

void SettingsPageWidget::resetToDefault()
{
    bool result = BasicDialogs::question(this, QString::fromUtf8("Подтверждение"),
                                         QString::fromUtf8("Все настройки программы будут удалены. Вы действительно хотите продолжить?"));

    if (!result)
        return;

    SnmpConfig::toDefault();
    SwitchConfig::toDefault();
    DslamProfileConfig::toDefault();

    initSnmpTabFields();
    initDevicesTabFields();

    showMessage(trUtf8("Настройки сброшены! Нажмите \"Сохранить\" чтобы применить изменения"));
}

bool SettingsPageWidget::validateSettingsData()
{
    if (ui->readComEdit->text().trimmed().isEmpty()) {
        showMessage(trUtf8("Введите комьюнити для чтения!"));
        ui->readComEdit->setFocus();
        return false;
    }

    if (ui->writeComEdit->text().trimmed().isEmpty()) {
        showMessage(trUtf8("Введите комьюнити для записи!"));
        ui->writeComEdit->setFocus();
        return false;
    }

    if (ui->timeoutEdit->text().trimmed().isEmpty()) {
        showMessage(trUtf8("Введите значение таймаута!"));
        ui->timeoutEdit->setFocus();
        return false;
    }

    if (ui->saveTimeoutEdit->text().trimmed().isEmpty()) {
        showMessage(trUtf8("Введите значение таймаута сохранения конфигурации на коммутаторе!"));
        ui->saveTimeoutEdit->setFocus();
        return false;
    }

    if (ui->portEdit->text().trimmed().isEmpty()) {
        showMessage(trUtf8("Введите значение TCP порта!"));
        ui->portEdit->setFocus();
        return false;
    }

    if (ui->retriesEdit->text().trimmed().isEmpty()) {
        showMessage(trUtf8("Введите значение количества повторов!"));
        ui->retriesEdit->setFocus();
        return false;
    }

    if (ui->swInternetVlanEdit->text().trimmed().isEmpty()) {
        showMessage(trUtf8("Введите названия влана для интернет трафика!"));
        ui->swInternetVlanEdit->setFocus();
        return false;
    }

    if (ui->swIptvUnicastVlanEdit->text().trimmed().isEmpty()) {
        showMessage(trUtf8("Введите названия влана для юникастового IPTV трафика!"));
        ui->swIptvUnicastVlanEdit->setFocus();
        return false;
    }

    return true;
}

void SettingsPageWidget::currentItemChanged(int item)
{
    ui->stackedWidget->setCurrentIndex(item);
}

void SettingsPageWidget::addDslProfile()
{
    DslProfileTableModel *model = qobject_cast<DslProfileTableModel *>(ui->dsDslProfileTableView->model());

    if (!model)
        return;

    int row = model->rowCount(QModelIndex());

    model->insertRow(row, QModelIndex());

    QModelIndex index = model->index(row, 0);
    ui->dsDslProfileTableView->scrollToBottom();
    ui->dsDslProfileTableView->setCurrentIndex(index);
    ui->dsDslProfileTableView->setFocus();
    ui->dsDslProfileTableView->edit(index);
}

void SettingsPageWidget::editDslProfile()
{
    if (!ui->dsDslProfileTableView->model())
        return;

    ui->dsDslProfileTableView->setFocus();
    ui->dsDslProfileTableView->edit(ui->dsDslProfileTableView->currentIndex());
}

void SettingsPageWidget::removeDslProfile()
{
    DslProfileTableModel *model = qobject_cast<DslProfileTableModel *>(ui->dsDslProfileTableView->model());

    if (!model)
        return;

    QModelIndex index = ui->dsDslProfileTableView->currentIndex();

    if (!index.isValid())
        return;

    QModelIndex displayNameIndex = model->index(index.row(), 0);
    QString name = model->data(displayNameIndex).toString();

    if (!BasicDialogs::okToDelete(this, QString::fromUtf8("Удаление DSL профиля"),
                                  QString(QString::fromUtf8("Вы действительно хотите удалить профиль [%1]?")).arg(name)))
        return;

    model->removeRow(index.row(), QModelIndex());
}

void SettingsPageWidget::dsComboBoxCurrentIndexChanged(int index)
{
    Q_UNUSED(index);

    if (ui->dsDeviceModelComboBox->currentIndex() == -1
            || ui->dsTypeBoardComboBox->currentIndex() == -1)
        return ui->dsDslProfileTableView->setModel(0);

    DeviceModel::Enum deviceModel = DeviceModel::from(ui->dsDeviceModelComboBox->currentText());
    bool isAdsl = ui->dsTypeBoardComboBox->currentIndex() == 0;

    DslProfileTableModel *model = 0;
    if (isAdsl) {
        model = DslamProfileConfig::adsl(deviceModel);
    } else {
        model = DslamProfileConfig::shdsl(deviceModel);
    }

    ui->dsDslProfileTableView->setModel(model);
    ui->dsDslProfileTableView->setColumnWidth(0, 200);
    ui->dsDslProfileTableView->setColumnWidth(1, 200);
}

