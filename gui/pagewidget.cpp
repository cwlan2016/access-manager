#include "pagewidget.h"

PageWidget::PageWidget(Device::Ptr deviceInfo, QWidget *parent) :
    QWidget(parent), mDeviceInfo(deviceInfo)
{
}
