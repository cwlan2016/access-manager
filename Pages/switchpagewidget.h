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
private:
    void changeStateSwitchPortInMulticastVlan(bool state);
public slots:
    void saveSwitchConfig();
private slots:
    void showPortInfoGroupBox();
    void refreshPortInfo();
    void refreshAllPortInfo();
    void refreshMacTable();
    void portViewRequestContextMenu(QPoint point);
    void macTableViewRequestContextMenu(QPoint point);
    void addPortToMulticastVlan();
    void removePortFromMulticastVlan();
    void setPortInternetService();
    void setPortInternetWithStbService();
    void filterMacAddressByPorts();
    void macLineEditTextChanged(QString text);
    void macRadioButtonChangeState(bool checked);
private:
    Ui::SwitchPageWidget *ui;
};

#endif // SWITCHPAGEWIDGET_H
