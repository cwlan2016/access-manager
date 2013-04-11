#ifndef PAGEWIDGET_H
#define PAGEWIDGET_H

#ifdef _MSC_VER
#include "../devices/device.h"
#else
#include "devices/device.h"
#endif

class PageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PageWidget(Device::Ptr deviceInfo, QWidget *parent = 0);

protected:
    Device::Ptr mDeviceInfo;
};

#endif // PAGEWIDGET_H
