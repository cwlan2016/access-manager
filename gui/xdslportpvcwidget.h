#ifndef XDSLPORTPVCWIDGET_H
#define XDSLPORTPVCWIDGET_H

#include <QWidget>

namespace Ui {
class XdslPortPvcWidget;
}

class XdslPortPvcWidget : public QWidget
{
    Q_OBJECT

public:
    explicit XdslPortPvcWidget(QWidget *parent = 0);
    ~XdslPortPvcWidget();

private:
    Ui::XdslPortPvcWidget *ui;
};

#endif // XDSLPORTPVCWIDGET_H
