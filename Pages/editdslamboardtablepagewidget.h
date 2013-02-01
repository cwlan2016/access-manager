#ifndef EDITDSLAMBOARDTABLEPAGEWIDGET_H
#define EDITDSLAMBOARDTABLEPAGEWIDGET_H

#include "pagewidget.h"
#ifdef _MSC_VER
#include "../Models/devicetablemodel.h"
#else
#include "Models/devicetablemodel.h"
#endif

namespace Ui
{
class EditDslamBoardTablePageWidget;
}

class EditDslamBoardTablePageWidget : public PageWidget
{
    Q_OBJECT

public:
    explicit EditDslamBoardTablePageWidget(DeviceInfo::Ptr deviceInfo, DeviceTableModel *deviceListModel, QWidget *parent = 0);
    ~EditDslamBoardTablePageWidget();
private:
    void editBoardViewRequestContextMenu(QPoint point);
    void renumeringBoardPairs();
    void getBoardList();
    void editBoard();
    void removeBoard();
    void autoUpdateBoardListStateChanged(bool state);
    void autoNumeringPairsStateChanged(bool state);

    Ui::EditDslamBoardTablePageWidget *ui;
    DeviceTableModel *mDeviceListModel;
};

#endif // EDITDSLAMBOARDTABLEPAGEWIDGET_H
