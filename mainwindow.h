#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QModelIndex>
#include <QtWidgets/QMainWindow>
#include "customtypes.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void createDeviceListPage();
    void loadDeviceList();
    void loadProgramSettings();
private:
    Ui::MainWindow              *ui;
    QVector<PageType::Enum>     *mTypePageList;
    QHash<QString, QWidget *>   *mPageList;

    void closeEvent(QCloseEvent *event);
    //Window actions
    void showAboutProgramPage();
    void showSettingsPage();
    void tabCloseRequested(int index);
    void tabCurrentChanged(int index);
    //DeviceTablePage actions
    void deviceViewActivatedItem(QModelIndex index);
    //SwitchTablePage actions
    void saveSwitchConfig();
    //DslamTablePage actions
    void upDslPort();
    void downDslPort();
};

#endif // MAINWINDOW_H
