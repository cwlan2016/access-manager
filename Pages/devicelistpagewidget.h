#ifndef DEVICELISTPAGEWIDGET_H
#define DEVICELISTPAGEWIDGET_H

#include <QtCore/QSortFilterProxyModel>
#include <QtWidgets/QTableWidget>
#ifdef _MSC_VER
#include "../Models/devicelistmodel.h"
#else
#include "Models/devicelistmodel.h"
#endif

namespace Ui
{
class DeviceListPageWidget;
}

class DeviceListPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceListPageWidget(QTabWidget *mParentTabWidget,
                                  QVector<PageType::Enum> *typePageList,
                                  QHash<QString, QWidget *> *pageList, QWidget *parent = 0);
    ~DeviceListPageWidget();
    DeviceListModel *deviceListModel()  const;
    QSortFilterProxyModel *proxyModel() const;
    QModelIndex currentDeviceListItem() const;
    void clearSelection();
private:
    Ui::DeviceListPageWidget   *ui;
    DeviceListModel            *mDeviceListModel;
    QSortFilterProxyModel      *mProxyModel;
    QVector<PageType::Enum>    *mTypePageList;
    QHash<QString, QWidget *>  *mPageList;
    QTabWidget                 *mParentTabWidget;
    void batchUpdate(DeviceType::Enum updatingDeviceType);
signals:
    void changedActiveItem(QModelIndex index);
public slots:
    void loadDeviceList();
    void saveDeviceList();
    void openDevice();
    void addDevice();
    void editDevice();
    void removeDevice();
    void updateVlanInfoSwitch();
    void updateBoardInfoDslam();
    void updateProfileInfoOlt();
    void batchUpdateBoardsInfoDslam();
    void batchUpdateVlanInfoSwitch();
    void batchUpdateProfileOlt();
    void batchUpdateInfoAllDevices();
    void showEditDslamBoardListPage();
    void showVlanInfoGroupBox();
    void showProfileInfoGroupBox();
private slots:
    void filterDeviceTextChanged(QString text);
    void deviceViewRequestContextMenu(QPoint point);
    void vlanInfoGBoxStateChanged(bool state);
    void profileInfoGboxStateChanged(bool state);
    void viewActivatedItem(QModelIndex currIndex, QModelIndex prevIndex);
};

#endif // DEVICELISTPAGEWIDGET_H
