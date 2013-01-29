#include "settingspagewidget.h"
#include "ui_settingspagewidget.h"

#ifdef _MSC_VER
#include "../basicdialogs.h"
#include "../config.h"
#include "../constant.h"
#else
#include "basicdialogs.h"
#include "config.h"
#include "constant.h"
#endif

SettingsPageWidget::SettingsPageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsPageWidget)
{
    ui->setupUi(this);

    ui->readComEdit->setText(SnmpConfigInfo::readCommunity());
    ui->writeComEdit->setText(SnmpConfigInfo::writeCommunity());
    ui->timeoutEdit->setText(QString::number(SnmpConfigInfo::timeout()));
    ui->timeoutEdit->setValidator(new QIntValidator(0, 100000));
    ui->saveTimeoutEdit->setText(QString::number(SnmpConfigInfo::saveConfigTimeout()));
    ui->saveTimeoutEdit->setValidator(new QIntValidator(0, 100000));
    ui->portEdit->setText(QString::number(SnmpConfigInfo::port()));
    ui->portEdit->setValidator(new QIntValidator(0, 100000));
    ui->retriesEdit->setText(QString::number(SnmpConfigInfo::retries()));
    ui->retriesEdit->setValidator(new QIntValidator(0, 100000));

    ui->stackedWidget->setCurrentWidget(ui->snmpPage);

    connect(ui->saveSettingsButton, &QPushButton::pressed,
            this, &SettingsPageWidget::saveSetting);
}

SettingsPageWidget::~SettingsPageWidget()
{
    delete ui;
}

void SettingsPageWidget::saveSetting()
{
    if (!validateSettingsData())
        return;

    SnmpConfigInfo::setReadCommunity(ui->readComEdit->text());
    SnmpConfigInfo::setWriteCommunity(ui->writeComEdit->text());
    SnmpConfigInfo::setTimeout(ui->timeoutEdit->text().toUInt());
    SnmpConfigInfo::setSaveConfigTimeout(ui->saveTimeoutEdit->text().toUInt());
    SnmpConfigInfo::setPort(ui->portEdit->text().toUInt());
    SnmpConfigInfo::setRetries(ui->retriesEdit->text().toUInt());

    if (!Config::save()) {
        BasicDialogs::error(this, BasicDialogTitle::Error, Config::errorString(), "");
        return;
    } else {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Настройки программы сохранены."), "");
        return;
    }
}

bool SettingsPageWidget::validateSettingsData()
{
    if (ui->readComEdit->text().trimmed() == "") {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Введите комьюнити для чтения!"));
        ui->readComEdit->setFocus();
        return false;
    }

    if (ui->writeComEdit->text().trimmed() == "") {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Введите комьюнити для записи!"));
        ui->writeComEdit->setFocus();
        return false;
    }


    if (ui->timeoutEdit->text().trimmed() == "") {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Введите значение таймаута!"));
        ui->timeoutEdit->setFocus();
        return false;
    }

    if (ui->saveTimeoutEdit->text().trimmed() == "") {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Введите значение таймаута сохранения конфигурации на коммутаторе!"));
        ui->saveTimeoutEdit->setFocus();
        return false;
    }

    if (ui->portEdit->text().trimmed() == "") {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Введите значение TCP порта!"));
        ui->portEdit->setFocus();
        return false;
    }

    if (ui->retriesEdit->text().trimmed() == "") {
        BasicDialogs::information(this, BasicDialogTitle::Info, QString::fromUtf8("Введите значение количества повторов!"));
        ui->retriesEdit->setFocus();
        return false;
    }

    return true;
}
