#include "pagewidget.h"

PageWidget::PageWidget(Device::Ptr device, QWidget *parent) :
    QWidget(parent), mDevice(device)
{
}

Device::Ptr PageWidget::device()
{
    return mDevice;
}
