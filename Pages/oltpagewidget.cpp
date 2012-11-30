#include "oltpagewidget.h"
#include "ui_oltpagewidget.h"

OltPageWidget::OltPageWidget(DeviceInfo::Ptr deviceInfo, QWidget* parent) :
    PageWidget(deviceInfo, parent),
    ui(new Ui::OltPageWidget)
{
    ui->setupUi(this);
}

OltPageWidget::~OltPageWidget()
{
    delete ui;
}
