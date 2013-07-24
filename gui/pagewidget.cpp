#include "pagewidget.h"

PageWidget::PageWidget(QWidget *parent) :
    QWidget(parent),
    mContextMenu(new QMenu(this))
{
}

void PageWidget::init()
{

}

QMenu *PageWidget::contextMenu()
{
    return mContextMenu;
}
