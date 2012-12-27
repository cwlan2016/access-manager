#ifndef EDITDSLAMBOARDLISTPAGEWIDGET_H
#define EDITDSLAMBOARDLISTPAGEWIDGET_H

#include "pagewidget.h"
#include "Info/dslaminfo.h"
#include "Models/boardlistdelegate.h"
#include "Models/devicelistmodel.h"

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
private slots:
    void editBoardViewRequestContextMenu(QPoint point);
    void renumeringBoardPairs();
    void getBoardList();
    void editBoard();
    void removeBoard();
    void autoUpdateBoardListStateChanged(bool state);
    void autoNumeringPairsStateChanged(bool state);
private:
    Ui::EditDslamBoardListPageWidget *ui;

    DeviceListModel *mDeviceListModel;
};

#endif // EDITDSLAMBOARDLISTPAGEWIDGET_H
