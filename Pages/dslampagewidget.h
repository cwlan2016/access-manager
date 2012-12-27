#ifndef DSLAMPAGEWIDGET_H
#define DSLAMPAGEWIDGET_H

#include "pagewidget.h"
#include "Info/dslaminfo.h"
#include "Models/boardlistmodel.h"
#include "Models/dslamportlistmodel.h"

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

private:
    Ui::DslamPageWidget *ui;

    void fillSelectProfileComboBox();
    QModelIndex currentDslamXdslPort();
public slots:
    void upDslPort();
    void downDslPort();
private slots:
    void viewRequestContextMenu(QPoint point);
    void showPortListModel();
    void backToBoardListModel();
    void showSelectProfileGBox();
    void applyDslProfile();
    void refreshPortInfo();
    void refreshAllPortInfo();
    void portListExpandedNode(QModelIndex index);
};

#endif // DSLAMPAGEWIDGET_H
