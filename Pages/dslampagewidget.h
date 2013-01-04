#ifndef DSLAMPAGEWIDGET_H
#define DSLAMPAGEWIDGET_H

#include <QtCore/QModelIndex>
#include <QtCore/QPoint>
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
    void viewRequestContextMenu(QPoint point);
    void showPortListModel();
    void backToBoardListModel();
    void showSelectProfileGBox();
    void applyDslProfile();
    void refreshPortInfo();
    void refreshAllPortInfo();
    void portListExpandedNode(QModelIndex index);
    void fillSelectProfileComboBox();
    QModelIndex currentDslamXdslPort();

    Ui::DslamPageWidget *ui;
};

#endif // DSLAMPAGEWIDGET_H
