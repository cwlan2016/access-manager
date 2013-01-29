#ifndef SETTINGSPAGEWIDGET_H
#define SETTINGSPAGEWIDGET_H

#include <QtWidgets/QWidget>

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

    Ui::SettingsPageWidget *ui;
};

#endif // SETTINGSPAGEWIDGET_H
