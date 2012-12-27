#ifndef PAGEWIDGET_H
#define PAGEWIDGET_H

#include "Info/deviceinfo.h"

class PageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PageWidget(DeviceInfo::Ptr deviceInfo, QWidget *parent = 0);
protected:
    DeviceInfo::Ptr mDeviceInfo;
signals:

public slots:

};

#endif // PAGEWIDGET_H
