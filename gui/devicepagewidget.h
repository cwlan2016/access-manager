#ifndef DEVICEPAGEWIDGET_H
#define DEVICEPAGEWIDGET_H

#include "pagewidget.h"
#include <devices/device.h>

class DevicePageWidget : public PageWidget
{
    Q_OBJECT
public:
    explicit DevicePageWidget(Device::Ptr device, QWidget *parent = 0);

    Device::Ptr device();

protected:
    Device::Ptr mDevice;
};

#endif // DEVICEPAGEWIDGET_H
