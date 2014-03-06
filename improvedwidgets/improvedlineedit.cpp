/****************************************************************************
**
** Copyright (c) 2007 Trolltech ASA <info@trolltech.com>
**
** Use, modification and distribution is allowed without limitation,
** warranty, liability or support of any kind.
**
****************************************************************************/

#include "improvedlineedit.h"

#include <QStyle>

ImprovedLineEdit::ImprovedLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    clearButton = new QToolButton(this);
    clearButton->setIcon(QIcon(":/images/clear_left.png"));
    clearButton->setIconSize(QSize(18, 18));
    clearButton->setCursor(Qt::ArrowCursor);
    clearButton->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    clearButton->setToolTip(trUtf8("Очистить текст"));
    clearButton->hide();

    connect(clearButton, &QToolButton::clicked,
            this, &ImprovedLineEdit::clear);
    connect(this, &ImprovedLineEdit::textChanged,
            this, &ImprovedLineEdit::updateClearButton);

    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
#ifdef Q_WS_MAC
    setAttribute(Qt::WA_MacShowFocusRect, 0);
#endif
    QSize msz = minimumSizeHint();
    setMinimumSize(qMax(msz.width(), clearButton->sizeHint().width() + frameWidth * 2 + 2),
                   qMax(msz.height(), clearButton->sizeHint().height() + frameWidth * 2 + 2));
}

void ImprovedLineEdit::resizeEvent(QResizeEvent *)
{
    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    QSize sz = clearButton->sizeHint();
    clearButton->move(rect().right() - frameWidth - sz.width(),
                      (rect().bottom() + 2 - sz.height()) / 2);
}

void ImprovedLineEdit::updateClearButton(const QString &text)
{
    if (!inputMask().isEmpty()) {
        QString temp = text;
        clearButton->setVisible(!temp.remove('-').isEmpty());
    } else {
        clearButton->setVisible(!text.isEmpty());
    }
}
