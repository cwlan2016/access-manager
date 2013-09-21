#ifndef DEVICETABLEPAGEWIDGET_H
#define DEVICETABLEPAGEWIDGET_H

#include "pagewidget.h"
#include <models/devicetablemodel.h>

namespace Ui
{
class DeviceTablePageWidget;
}

class DeviceTablePageWidget : public PageWidget
{
    Q_OBJECT

public:
    explicit DeviceTablePageWidget(QTabWidget *mParentTabWidget,
                                   QVector<PageType::Enum> *typePageList,
                                   QHash<QString, QWidget *> *pageList,
                                   QWidget *parent = 0);
    ~DeviceTablePageWidget();

    void init();

    void loadDeviceList();
    void saveDeviceList();

    void getServiceDataFromCurrentDevice();

    void batchUpdateBoardsDslam();
    void batchUpdateVlansSwitch();
    void batchUpdateProfilesOlt();
    void batchUpdateInfoAllDevices();

    void clearSelection();

    DeviceTableModel *deviceTableModel() const;

private:
    void openDevice();
    void addDevice();
    void editDevice();
    void removeDevice();

    void showSwitchExtInfoFrame();
    void showOltExtInfoFrame();
    void showExtendedInfoPanel();

    void initActions();
    void initComponents();
    void initMenu();
    void initView();

    void batchUpdate(DeviceType::Enum updatingDeviceType);

    void filterDeviceTextChanged(QString text);

    void deviceViewRequestContextMenu(QPoint point);
    void viewActivatedItem(QModelIndex currIndex, QModelIndex prevIndex);

    Ui::DeviceTablePageWidget  *ui;
    DeviceTableModel           *mDeviceTableModel;
    QSortFilterProxyModel      *mProxyModel;
    QHash<QString, QWidget *>  *mPageList;
    QVector<PageType::Enum>    *mTypePageList;
    QTabWidget                 *mParentTabWidget;
};

#endif // DEVICETABLEPAGEWIDGET_H
