#include "xdslportpvcwidget.h"
#include "ui_xdslportpvcwidget.h"

XdslPortPvcWidget::XdslPortPvcWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::XdslPortPvcWidget)
{
    ui->setupUi(this);
}

XdslPortPvcWidget::~XdslPortPvcWidget()
{
    delete ui;
}
