#include "oltpagewidget.h"
#include "ui_oltpagewidget.h"

OltPageWidget::OltPageWidget(DeviceInfo::Ptr deviceInfo, QWidget* parent) :
    PageWidget(deviceInfo, parent),
    ui(new Ui::OltPageWidget)
{
    ui->setupUi(this);

    ui->userSettingsGroupBox->setVisible(false);

    OntTableModel* ontTableModel = new OntTableModel(std::static_pointer_cast<OltInfo>(deviceInfo), this);
    ontTableModel->setIp(deviceInfo->ip());

    if (!ontTableModel->load())
    {
        BasicDialogs::error(this, BasicDialogTitle::Error, ontTableModel->error());
    }

    QSortFilterProxyModel* ontTableFilterProxyModel = new QSortFilterProxyModel(this);
    ontTableFilterProxyModel->setFilterRole(Qt::DisplayRole);
    ontTableFilterProxyModel->setFilterKeyColumn(1);
    ontTableFilterProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ontTableFilterProxyModel->setSourceModel(ontTableModel);

    ui->ontListTableView->setModel(ontTableFilterProxyModel);
}

OltPageWidget::~OltPageWidget()
{
    delete ui;
}
