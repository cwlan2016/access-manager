#ifndef OLTPAGEWIDGET_H
#define OLTPAGEWIDGET_H

#include "stdafx.h"

#include "basicdialogs.h"
#include "pagewidget.h"
#include "Models/onttablemodel.h"

namespace Ui {
class OltPageWidget;
}

class OltPageWidget : public PageWidget
{
    Q_OBJECT
    
public:
    explicit OltPageWidget(DeviceInfo::Ptr deviceInfo, QWidget* parent = 0);
    ~OltPageWidget();
    
private:
    Ui::OltPageWidget*  ui;
};

#endif // OLTPAGEWIDGET_H
