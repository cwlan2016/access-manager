#ifndef SWITCHPAGEWIDGET_H
#define SWITCHPAGEWIDGET_H

#include "pagewidget.h"

class LineEdit;

namespace Ui
{
class SwitchPageWidget;
}

class SwitchPageWidget : public PageWidget
{
    Q_OBJECT

public:
    explicit SwitchPageWidget(DeviceInfo::Ptr deviceInfo, QWidget *parent = 0);
    ~SwitchPageWidget();

    void saveSwitchConfig();

private:
    void refreshPortInfo();
    void refreshAllPortInfo();
    void refreshMacTable();

    void addPortToMulticastVlan();
    void removePortFromMulticastVlan();
    void changeStateSwitchPortInMulticastVlan(bool state);

    void setPortInternetService();
    void setPortInternetWithStbService();

    void filterMacAddressByPorts();
    void macLineEditTextChanged(QString text);
    void macRadioButtonChangeState(bool checked);

    void showPortInfoGroupBox();
    void portViewRequestContextMenu(QPoint point);
    void macTableViewRequestContextMenu(QPoint point);

    Ui::SwitchPageWidget *ui;
};

#endif // SWITCHPAGEWIDGET_H
