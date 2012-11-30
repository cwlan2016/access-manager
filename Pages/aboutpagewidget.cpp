#include "aboutpagewidget.h"
#include "ui_aboutpagewidget.h"

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
