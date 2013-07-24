#include "oltpagewidget.h"
#include "ui_oltpagewidget.h"

#include <basicdialogs.h>
#include <constant.h>
#include <models/onttablemodel.h>

OltPageWidget::OltPageWidget(Device::Ptr deviceInfo, QWidget *parent) :
    DevicePageWidget(deviceInfo, parent),
    ui(new Ui::OltPageWidget)
{
    ui->setupUi(this);

    ui->userSettingsFrame->setVisible(false);

    OntTableModel *ontTableModel = new OntTableModel(deviceInfo.objectCast<Olt>(), this);

    if (!ontTableModel->load()) {
        BasicDialogs::error(parent, BasicDialogStrings::Error, ontTableModel->error());
    }

    QSortFilterProxyModel *ontTableFilterProxyModel = new QSortFilterProxyModel(this);
    ontTableFilterProxyModel->setFilterRole(Qt::DisplayRole);
    ontTableFilterProxyModel->setFilterKeyColumn(0);
    ontTableFilterProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ontTableFilterProxyModel->setSourceModel(ontTableModel);

    ui->ontListTableView->setModel(ontTableFilterProxyModel);

    connect(ui->filterOntComboBox, static_cast<void (QComboBox:: *)(int)>(&QComboBox::currentIndexChanged),
            this, &OltPageWidget::filterOntComboBoxIndexChanged);
    connect(ui->filterOntLineEdit, &QLineEdit::textChanged,
            this, &OltPageWidget::filterOntEditTextChanged);
}

OltPageWidget::~OltPageWidget()
{
    delete ui;
}

void OltPageWidget::filterOntComboBoxIndexChanged(int index)
{
    ui->filterOntLineEdit->clear();
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel *>(ui->ontListTableView->model());

    if (index == 0) {
        proxyModel->setFilterKeyColumn(0);
        ui->filterOntLineEdit->setPlaceholderText(QString::fromUtf8("Фильтрация информации по MAC-адресу / Серийному номеру..."));
    } else {
        proxyModel->setFilterKeyColumn(3);
        ui->filterOntLineEdit->setPlaceholderText(QString::fromUtf8("Фильтрация информации по описанию..."));
    }
}

void OltPageWidget::filterOntEditTextChanged(QString text)
{
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel *>(ui->ontListTableView->model());
    proxyModel->setFilterWildcard(text % "*");
}
