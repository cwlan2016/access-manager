#include "settingspagewidget.h"
#include "ui_settingspagewidget.h"

#include <basicdialogs.h>
#include <config.h>
#include <constant.h>

SettingsPageWidget::SettingsPageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsPageWidget)
{
    ui->setupUi(this);

    ui->readComEdit->setText(SnmpConfig::readCommunity());

    ui->readComEdit->setValidator(new QRegExpValidator(QRegExp("\\w+"), this));
    ui->writeComEdit->setText(SnmpConfig::writeCommunity());
    ui->timeoutEdit->setText(QString::number(SnmpConfig::timeout()));
    ui->timeoutEdit->setValidator(new QIntValidator(0, 100000, this));
    ui->saveTimeoutEdit->setText(QString::number(SnmpConfig::saveConfigTimeout()));
    ui->saveTimeoutEdit->setValidator(new QIntValidator(0, 100000, this));
    ui->portEdit->setText(QString::number(SnmpConfig::port()));
    ui->portEdit->setValidator(new QIntValidator(0, 100000, this));
    ui->retriesEdit->setText(QString::number(SnmpConfig::retries()));
    ui->retriesEdit->setValidator(new QIntValidator(0, 100000, this));

    ui->swInternetVlanEdit->setText(SwitchConfig::inetVlanName());
    ui->swIptvUnicastVlanEdit->setText(SwitchConfig::iptvVlanName());

    ui->stackedWidget->setCurrentIndex(0);
    ui->listWidget->setCurrentRow(0);

    connect(ui->saveSettingsButton, &QPushButton::pressed,
            this, &SettingsPageWidget::saveSetting);
    connect(ui->listWidget, &QListWidget::currentRowChanged,
            this, &SettingsPageWidget::currentItemChanged);
}

SettingsPageWidget::~SettingsPageWidget()
{
    delete ui;
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
        BasicDialogs::error(this, BasicDialogStrings::Error, Config::error(), "");
        return;
    } else {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Настройки программы сохранены."), "");
        return;
    }
}

bool SettingsPageWidget::validateSettingsData()
{
    if (ui->readComEdit->text().trimmed().isEmpty()) {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Введите комьюнити для чтения!"));
        ui->readComEdit->setFocus();
        return false;
    }

    if (ui->writeComEdit->text().trimmed().isEmpty()) {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Введите комьюнити для записи!"));
        ui->writeComEdit->setFocus();
        return false;
    }

    if (ui->timeoutEdit->text().trimmed().isEmpty()) {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Введите значение таймаута!"));
        ui->timeoutEdit->setFocus();
        return false;
    }

    if (ui->saveTimeoutEdit->text().trimmed().isEmpty()) {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Введите значение таймаута сохранения конфигурации на коммутаторе!"));
        ui->saveTimeoutEdit->setFocus();
        return false;
    }

    if (ui->portEdit->text().trimmed().isEmpty()) {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Введите значение TCP порта!"));
        ui->portEdit->setFocus();
        return false;
    }

    if (ui->retriesEdit->text().trimmed().isEmpty()) {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Введите значение количества повторов!"));
        ui->retriesEdit->setFocus();
        return false;
    }

    if (ui->swInternetVlanEdit->text().trimmed().isEmpty()) {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Введите названия влана для интернет трафика!"));
        ui->swInternetVlanEdit->setFocus();
        return false;
    }

    if (ui->swIptvUnicastVlanEdit->text().trimmed().isEmpty()) {
        BasicDialogs::information(this, BasicDialogStrings::Info, QString::fromUtf8("Введите названия влана для юникастового IPTV трафика!"));
        ui->swIptvUnicastVlanEdit->setFocus();
        return false;
    }

    return true;
}

void SettingsPageWidget::currentItemChanged(int item)
{
    ui->stackedWidget->setCurrentIndex(item);
}

