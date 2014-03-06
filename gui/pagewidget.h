#ifndef PAGEWIDGET_H
#define PAGEWIDGET_H

#include <stdafx.h>

class PageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PageWidget(QWidget *parent = 0);

    virtual void init();
    virtual QMenu *contextMenu();

protected:
    QMenu *mContextMenu; //fillInChild

};

#endif // PAGEWIDGET_H
