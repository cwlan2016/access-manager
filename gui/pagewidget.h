#ifndef PAGEWIDGET_H
#define PAGEWIDGET_H

#include <devices/device.h>

class PageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PageWidget(Device::Ptr device, QWidget *parent = 0);

    Device::Ptr device();

protected:
    Device::Ptr mDevice;
};

#endif // PAGEWIDGET_H
