#include "aboutpagewidget.h"
#include "ui_aboutpagewidget.h"

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/version.h>

AboutPageWidget::AboutPageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutPageWidget)
{
    ui->setupUi(this);

    ui->programVersionDescLabel->setText(qApp->applicationVersion());
    ui->copyrightDescLabel->setText(qApp->organizationName());
    ui->qtVersionDescLabel->setText(QT_VERSION_STR);
    ui->netSnmpVersionDescLabel->setText(netsnmp_get_version());
}

AboutPageWidget::~AboutPageWidget()
{
    delete ui;
}
