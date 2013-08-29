#ifndef SWITCHPAGEWIDGET_H
#define SWITCHPAGEWIDGET_H

#include "devicepagewidget.h"

class LineEdit;

namespace Ui
{
class SwitchPageWidget;
}

class SwitchPageWidget : public DevicePageWidget
{
    Q_OBJECT

public:
    explicit SwitchPageWidget(Device::Ptr deviceInfo, QWidget *parent = 0);
    ~SwitchPageWidget();

    void init();

private:
    void initActions();
    void initComponents();
    void initMenu();
    void initView();

    void saveSwitchConfig();

    void editDescPort();

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

    void showPortInfoFrame();
    void portViewRequestContextMenu(QPoint point);
    void macTableViewRequestContextMenu(QPoint point);

    void updateMacTableFinished(bool withErrors);

    Ui::SwitchPageWidget *ui;
};

#endif // SWITCHPAGEWIDGET_H
