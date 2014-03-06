/****************************************************************************
**
** Copyright (c) 2007 Trolltech ASA <info@trolltech.com>
**
** Use, modification and distribution is allowed without limitation,
** warranty, liability or support of any kind.
**
****************************************************************************/

#ifndef IMPROVEDLINEEDIT_H
#define IMPROVEDLINEEDIT_H

#include <QLineEdit>
#include <QToolButton>

class ImprovedLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit ImprovedLineEdit(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *);

private:
    void updateClearButton(const QString &text);

    QToolButton *clearButton;
};

#endif
