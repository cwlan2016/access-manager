#ifndef DEVICETABLEPAGEWIDGET_H
#define DEVICETABLEPAGEWIDGET_H

#ifdef _MSC_VER
#include "../models/devicetablemodel.h"
#else
#include "models/devicetablemodel.h"
#endif

namespace Ui
{
class DeviceTablePageWidget;
}

class DeviceTablePageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceTablePageWidget(QTabWidget *mParentTabWidget,
                                   QVector<PageType::Enum> *typePageList,
                                   QHash<QString, QWidget *> *pageList,
                                   QWidget *parent = 0);
    ~DeviceTablePageWidget();

    void loadDeviceList();
    void saveDeviceList();

    void openDevice();
    void addDevice();
    void editDevice();
    void removeDevice();

    void updateVlanInfoSwitch();
    void updateBoardInfoDslam();
    void updateProfileInfoOlt();

    void batchUpdateBoardsDslam();
    void batchUpdateVlansSwitch();
    void batchUpdateProfilesOlt();
    void batchUpdateInfoAllDevices();

    void showEditDslamBoardListPage();
    void showVlanInfoGroupBox();
    void showProfileInfoGroupBox();

    void clearSelection();

    DeviceTableModel *deviceTableModel() const;
    QSortFilterProxyModel *proxyModel() const;
    QModelIndex currentDeviceListItem() const;

signals:
    void changedActiveItem(QModelIndex index);

private:
    void batchUpdate(DeviceType::Enum updatingDeviceType);

    void filterDeviceTextChanged(QString text);
    void vlanInfoGBoxStateChanged(bool state);
    void profileInfoGboxStateChanged(bool state);

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
