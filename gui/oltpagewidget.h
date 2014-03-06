#ifndef OLTPAGEWIDGET_H
#define OLTPAGEWIDGET_H

#include "devicepagewidget.h"

#include <improvedwidgets/improvedmessagewidget.h>

namespace Ui
{
class OltPageWidget;
}

class OltPageWidget : public DevicePageWidget
{
    Q_OBJECT

public:
    explicit OltPageWidget(Device::Ptr deviceInfo, QWidget *parent = 0);
    ~OltPageWidget();

private:
    void showMessage(const QString &msg,
                     ImprovedMessageWidget::MessageType messageType = ImprovedMessageWidget::Information);
    void filterOntComboBoxIndexChanged(int index);
    void filterOntEditTextChanged(QString text);

    Ui::OltPageWidget  *ui;
};

#endif // OLTPAGEWIDGET_H
