#ifndef DSLAMPAGEWIDGET_H
#define DSLAMPAGEWIDGET_H

#include "pagewidget.h"

namespace Ui
{
class DslamPageWidget;
}

class DslamPageWidget : public PageWidget
{
    Q_OBJECT

public:
    explicit DslamPageWidget(DeviceInfo::Ptr deviceInfo, QWidget *parent = 0);
    ~DslamPageWidget();

    void upDslPort();
    void downDslPort();

private:
    void showPortListModel();
    void backToBoardListModel();

    void showSelectProfileGBox();
    void fillSelectProfileComboBox();
    void applyDslProfile();

    void refreshPortInfo();
    void refreshAllPortInfo();

    void viewRequestContextMenu(QPoint point);
    void portListExpandedNode(QModelIndex index);

    QModelIndex currentDslamXdslPort();

    Ui::DslamPageWidget *ui;
};

#endif // DSLAMPAGEWIDGET_H
