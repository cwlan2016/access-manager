#ifndef SETTINGSPAGEWIDGET_H
#define SETTINGSPAGEWIDGET_H

#include <stdafx.h>

#include <improvedwidgets/improvedmessagewidget.h>

namespace Ui
{
class SettingsPageWidget;
}

class SettingsPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPageWidget(QWidget *parent = 0);
    ~SettingsPageWidget();

private:
    void showMessage(const QString &msg,
                     ImprovedMessageWidget::MessageType messageType = ImprovedMessageWidget::Information);

    void initGlobalActions();
    void initSnmpTabFields();
    void initDevicesTabFields();
    void initDevicesTabActions();

    void saveSetting();
    void resetToDefault();
    bool validateSettingsData();
    void currentItemChanged(int item);

    void addDslProfile();
    void editDslProfile();
    void removeDslProfile();

    void dsComboBoxCurrentIndexChanged(int index);

    Ui::SettingsPageWidget *ui;
};

#endif // SETTINGSPAGEWIDGET_H
