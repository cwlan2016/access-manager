#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stdafx.h"
#include "config.h"
#include "constant.h"
#include "Pages/devicelistpagewidget.h"
#include "Pages/dslampagewidget.h"
#include "Pages/switchpagewidget.h"
#include "Pages/settingspagewidget.h"
#include "Pages/aboutpagewidget.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
    void createDeviceListPage();
    void loadDeviceList();
    void loadProgramSettings();
private:
    Ui::MainWindow*             ui;
    QVector<PageType>*          mTypePageList;
    QHash<QString, QWidget*>*   mPageList;

    void closeEvent(QCloseEvent* event);
private slots:
    //Действия окна
    void showAboutProgramPage();
    void showSettingsPage();
    void tabCloseRequested(int index);
    void tabCurrentChanged(int index);
    //Действия вкладки списка устройст
    void deviceViewActivatedItem(QModelIndex index);
    //Действия вкладки коммутатора
    void saveSwitchConfig();
    //Действия вкладки дслама
    void upDslPort();
    void downDslPort();
};

#endif // MAINWINDOW_H
