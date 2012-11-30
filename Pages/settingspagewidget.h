#ifndef SETTINGSPAGEWIDGET_H
#define SETTINGSPAGEWIDGET_H

#include "stdafx.h"

#include "config.h"
#include "constant.h"
#include "basicdialogs.h"

namespace Ui {
class SettingsPageWidget;
}

class SettingsPageWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SettingsPageWidget(QWidget* parent = 0);
    ~SettingsPageWidget();
private slots:
    void saveSetting();
private:
    bool validateSettingsData();

    Ui::SettingsPageWidget* ui;
};

#endif // SETTINGSPAGEWIDGET_H
