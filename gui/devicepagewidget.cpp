#include "devicepagewidget.h"

DevicePageWidget::DevicePageWidget(Device::Ptr device, QWidget *parent) :
    PageWidget(parent), mDevice(device)
{
}

Device::Ptr DevicePageWidget::device()
{
    return mDevice;
}
