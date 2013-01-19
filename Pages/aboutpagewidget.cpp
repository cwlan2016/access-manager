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

    QString qtVersion = QString("<a href=\"http://qt-project.org\">%1</a>").arg(QT_VERSION_STR);
    QString netSnmpVersion = QString("<A HREF=\"http://www.net-snmp.org\">%1</A>").arg(netsnmp_get_version());
    QString oxygenIcons = "<A HREF=\"http://www.oxygen-icons.org\">Oxygen</A>";

    ui->programVersionDescLabel->setText(qApp->applicationVersion());
    ui->copyrightDescLabel->setText(qApp->organizationName());
    ui->qtVersionDescLabel->setText(qtVersion);
    ui->netSnmpVersionDescLabel->setText(netSnmpVersion);
    ui->programIconsDescLabel->setText(oxygenIcons);
}

AboutPageWidget::~AboutPageWidget()
{
    delete ui;
}
