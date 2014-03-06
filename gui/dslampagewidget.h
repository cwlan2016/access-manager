#ifndef DSLAMPAGEWIDGET_H
#define DSLAMPAGEWIDGET_H

#include "devicepagewidget.h"

#include <improvedwidgets/improvedmessagewidget.h>
#include <models/boardtabledelegate.h>
#include <models/dslamporttabledelegate.h>

namespace Ui
{
class DslamPageWidget;
}

class DslamPageWidget : public DevicePageWidget
{
    Q_OBJECT

public:
    explicit DslamPageWidget(Device::Ptr deviceInfo, QWidget *parent = 0);
    ~DslamPageWidget();

    void init();

signals:
    void modeChanged(bool editMode);

private:
    void showMessage(const QString &msg,
                     ImprovedMessageWidget::MessageType messageType = ImprovedMessageWidget::Information,
                     const QString &detailedText = "");
    void initActions();
    void initComponents();
    void initView();

    void upDslPort();
    void downDslPort();
    void editDescPort();

    void showPortListModel();
    void backToBoardListModel();

    void fillSelectProfileComboBox();
    void applyDslProfile();

    void refreshPortInfo();
    void refreshAllPortInfo();

    void autoUpdateBoardListStateChanged(bool state);
    void autoNumeringPairsStateChanged(bool state);

    void editBoard();
    void removeBoard();

    void getBoardList();
    void renumeringPairs();

    void beginEditBoardList();
    void finishEditBoardList();
    void pageModeChanged(bool editMode);
    void currentColumnChanged(const QModelIndex &current, const QModelIndex &previous);

    void setupMenu();
    void setupView();

    void viewRequestContextMenu(QPoint point);
    void portListExpandedNode(QModelIndex index);

    void updatePortTableFinished(bool withErrors);

    QModelIndex currentDslamXdslPort();

    Ui::DslamPageWidget *ui;
    BoardTableDelegate *mBoardTableDelegate;
    DslamPortTableDelegate *mPortTableDelegate;
    bool mEditMode;
};

#endif // DSLAMPAGEWIDGET_H
