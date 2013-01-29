/****************************************************************************
**
** Copyright (c) 2007 Trolltech ASA <info@trolltech.com>
**
** Use, modification and distribution is allowed without limitation,
** warranty, liability or support of any kind.
**
****************************************************************************/

#include "lineedit.h"

#include <QtWidgets/QStyle>

LineEdit::LineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    clearButton = new QToolButton(this);
    QPixmap clearButtonPixmap(":/images/clear_left.png");
    clearButton->setIcon(QIcon(clearButtonPixmap));
    clearButton->setIconSize(clearButtonPixmap.size());
    clearButton->setCursor(Qt::ArrowCursor);
    clearButton->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    clearButton->setToolTip(QString::fromUtf8("Очистить текст"));
    clearButton->hide();
    connect(clearButton, &QToolButton::clicked,
            this, &LineEdit::clear);
    connect(this, &LineEdit::textChanged,
            this, &LineEdit::updateCloseButton);
    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
#ifdef Q_WS_MAC
    setAttribute(Qt::WA_MacShowFocusRect, 0);
#endif
    QSize msz = minimumSizeHint();
    setMinimumSize(qMax(msz.width(), clearButton->sizeHint().width() + frameWidth * 2 + 2),
                   qMax(msz.height(), clearButton->sizeHint().height() + frameWidth * 2 + 2));

}

void LineEdit::resizeEvent(QResizeEvent *)
{
    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    QSize sz = clearButton->sizeHint();
    clearButton->move(rect().right() - frameWidth - sz.width(),
                      (rect().bottom() + 2 - sz.height()) / 2);
}

void LineEdit::updateCloseButton(const QString &text)
{
    if (!inputMask().isEmpty()) {
        QString temp = text;
        clearButton->setVisible(!temp.remove('-').isEmpty());
    } else
        clearButton->setVisible(!text.isEmpty());
}
