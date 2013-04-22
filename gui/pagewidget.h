#ifndef PAGEWIDGET_H
#define PAGEWIDGET_H

#include <devices/device.h>

class PageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PageWidget(Device::Ptr deviceInfo, QWidget *parent = 0);

protected:
    Device::Ptr mDeviceInfo;
};

#endif // PAGEWIDGET_H
