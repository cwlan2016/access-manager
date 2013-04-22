#ifndef EDITDSLAMBOARDTABLEPAGEWIDGET_H
#define EDITDSLAMBOARDTABLEPAGEWIDGET_H

#include "pagewidget.h"
#include <models/devicetablemodel.h>

namespace Ui
{
class EditDslamBoardTablePageWidget;
}

class EditDslamBoardTablePageWidget : public PageWidget
{
    Q_OBJECT

public:
    explicit EditDslamBoardTablePageWidget(Device::Ptr deviceInfo,
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
};

#endif // EDITDSLAMBOARDTABLEPAGEWIDGET_H
