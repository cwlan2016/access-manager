#include "oltpagewidget.h"
#include "ui_oltpagewidget.h"

OltPageWidget::OltPageWidget(DeviceInfo::Ptr deviceInfo, QWidget *parent) :
    PageWidget(deviceInfo, parent),
    ui(new Ui::OltPageWidget)
{
    ui->setupUi(this);

    ui->userSettingsGroupBox->setVisible(false);

    OntTableModel *ontTableModel = new OntTableModel(std::static_pointer_cast<OltInfo>(deviceInfo), this);

    if (!ontTableModel->load()) {
        BasicDialogs::error(parent, BasicDialogTitle::Error, ontTableModel->error());
    }

    QSortFilterProxyModel *ontTableFilterProxyModel = new QSortFilterProxyModel(this);
    ontTableFilterProxyModel->setFilterRole(Qt::DisplayRole);
    ontTableFilterProxyModel->setFilterKeyColumn(0);
    ontTableFilterProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ontTableFilterProxyModel->setSourceModel(ontTableModel);

    ui->ontListTableView->setModel(ontTableFilterProxyModel);

    connect(ui->filterOntComboBox, SIGNAL(currentIndexChanged(int)), SLOT(filterOntCBoxIndexChanged(int)));
    connect(ui->filterOntLineEdit, SIGNAL(textChanged(QString)), SLOT(filterOntEditTextChanged(QString)));
}

OltPageWidget::~OltPageWidget()
{
    delete ui;
}

void OltPageWidget::filterOntCBoxIndexChanged(int index)
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
