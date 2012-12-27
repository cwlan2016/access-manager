#ifndef ABOUTPAGEWIDGET_H
#define ABOUTPAGEWIDGET_H

#include "QtWidgets/QWidget"

namespace Ui
{
class AboutPageWidget;
}

class AboutPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AboutPageWidget(QWidget *parent = 0);
    ~AboutPageWidget();

private:
    Ui::AboutPageWidget *ui;
};

#endif // ABOUTPAGEWIDGET_H
