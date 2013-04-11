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
    explicit EditDslamBoardTablePageWidget(Device::Ptr deviceInfo,
                                           DeviceTableModel *deviceListModel,
                                           QWidget *parent = 0);
    ~EditDslamBoardTablePageWidget();

private:
    void editBoard();
    void removeBoard();

    void getBoardList();
    void renumeringBoardPairs();

    void autoUpdateBoardListStateChanged(bool state);
    void autoNumeringPairsStateChanged(bool state);
    void editBoardViewRequestContextMenu(QPoint point);

    Ui::EditDslamBoardTablePageWidget *ui;
    DeviceTableModel *mDeviceTableModel;
};

#endif // EDITDSLAMBOARDTABLEPAGEWIDGET_H
