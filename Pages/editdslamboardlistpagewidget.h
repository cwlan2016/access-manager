#ifndef EDITDSLAMBOARDLISTPAGEWIDGET_H
#define EDITDSLAMBOARDLISTPAGEWIDGET_H

#include "pagewidget.h"
#ifdef _MSC_VER
#include "../Models/devicelistmodel.h"
#else
#include "Models/devicelistmodel.h"
#endif

namespace Ui
{
class EditDslamBoardListPageWidget;
}

class EditDslamBoardListPageWidget : public PageWidget
{
    Q_OBJECT

public:
    explicit EditDslamBoardListPageWidget(DeviceInfo::Ptr deviceInfo, DeviceListModel *deviceListModel, QWidget *parent = 0);
    ~EditDslamBoardListPageWidget();
private:
    void editBoardViewRequestContextMenu(QPoint point);
    void renumeringBoardPairs();
    void getBoardList();
    void editBoard();
    void removeBoard();
    void autoUpdateBoardListStateChanged(bool state);
    void autoNumeringPairsStateChanged(bool state);

    Ui::EditDslamBoardListPageWidget *ui;
    DeviceListModel *mDeviceListModel;
};

#endif // EDITDSLAMBOARDLISTPAGEWIDGET_H
