#ifndef SETTINGSPAGEWIDGET_H
#define SETTINGSPAGEWIDGET_H

#include <stdafx.h>

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
    void saveSetting();
    bool validateSettingsData();
    void currentItemChanged(int item);

    Ui::SettingsPageWidget *ui;
};

#endif // SETTINGSPAGEWIDGET_H
