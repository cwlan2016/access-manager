#ifndef OLTPAGEWIDGET_H
#define OLTPAGEWIDGET_H

#include "pagewidget.h"

namespace Ui
{
class OltPageWidget;
}

class OltPageWidget : public PageWidget
{
    Q_OBJECT

public:
    explicit OltPageWidget(DeviceInfo::Ptr deviceInfo, QWidget *parent = 0);
    ~OltPageWidget();

private:
    Ui::OltPageWidget  *ui;
private slots:
    void filterOntCBoxIndexChanged(int index);
    void filterOntEditTextChanged(QString text);
};

#endif // OLTPAGEWIDGET_H
