#include "improvedbusyindicator.h"

#include <QPainter>
#include <QPixmapCache>
#include <QGradient>

ImprovedBusyIndicator::ImprovedBusyIndicator(QWidget *parent) :
    QWidget(parent),
    mStartAngle(0),
    mStyle(StyleArc)
{
    QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    policy.setHeightForWidth(true);
    setSizePolicy(policy);

    mFillColor = palette().color(QPalette::WindowText);

    mTimer.setInterval(100);
    connect(&mTimer, SIGNAL(timeout()), this, SLOT(rotate()));
    mTimer.start();
}

void ImprovedBusyIndicator::rotate()
{
    mStartAngle += 30;
    mStartAngle %= 360;
    update();
}

void ImprovedBusyIndicator::setIndicatorStyle(IndicatorStyle style)
{
    mStyle = style;
    update();
}

void ImprovedBusyIndicator::setColor(QColor color)
{
    mFillColor = color;
}

void ImprovedBusyIndicator::setInterval(int interval)
{
    mTimer.setInterval(interval);
}

ImprovedBusyIndicator::IndicatorStyle ImprovedBusyIndicator::indicatorStyle() const
{
    return mStyle;
}

QColor ImprovedBusyIndicator::color() const
{
    return mFillColor;
}

int ImprovedBusyIndicator::interval() const
{
    return mTimer.interval();
}

QPixmap ImprovedBusyIndicator::generatePixmap(int side)
{
    QPixmap pixmap(QSize(side, side));
    pixmap.fill(QColor(255, 255, 255, 0));

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.translate(side / 2, side / 2);
    painter.scale(side / 200.0, side / 200.0);

    switch (mStyle) {
    case StyleRect:
        drawRectStyle(&painter);
        break;
    case StyleEllipse:
        drawEllipseStyle(&painter);
        break;
    case StyleArc:
        drawArcStyle(&painter);
        break;
    }

    return pixmap;
}

void ImprovedBusyIndicator::drawRectStyle(QPainter *painter)
{
    //    QColor color = palette().color(QPalette::WindowText);
    QColor color = mFillColor;
    QBrush brush(color);
    painter->setPen(Qt::NoPen);

    painter->rotate(mStartAngle);

    float angle = 0;
    while (angle < 360) {
        painter->setBrush(brush);
        painter->drawRect(-8, -100, 16, 35);

        painter->rotate(30);
        angle += 30;

        color.setAlphaF(angle / 360);
        brush.setColor(color);
    }
}

void ImprovedBusyIndicator::drawEllipseStyle(QPainter *painter)
{
    //    QColor color = palette().color(QPalette::WindowText);
    QColor color = mFillColor;
    QBrush brush(color);
    painter->setPen(Qt::NoPen);

    painter->rotate(mStartAngle);

    float angle = 0;
    while (angle < 360) {
        painter->setBrush(brush);
        painter->drawEllipse(-10, -100, 30, 30);

        painter->rotate(30);
        angle += 30;

        color.setAlphaF(angle / 360);
        brush.setColor(color);
    }
}

void ImprovedBusyIndicator::drawArcStyle(QPainter *painter)
{
    //    QColor color = palette().color(QPalette::WindowText);
    QColor color = mFillColor;
    QConicalGradient gradient(0, 0, -mStartAngle);
    gradient.setColorAt(0, color);
    color.setAlpha(0);
    gradient.setColorAt(0.8, color);
    color.setAlpha(255);
    gradient.setColorAt(1, color);

    QPen pen;
    pen.setWidth(30);
    pen.setBrush(QBrush(gradient));
    painter->setPen(pen);

    painter->drawArc(-85, -85, 170, 170, 0 * 16, 360 * 16);
}

void ImprovedBusyIndicator::paintEvent(QPaintEvent *)
{
    QString key = QString("%1:%2:%3:%4:%5")
            .arg(metaObject()->className())
            .arg(width())
            .arg(height())
            .arg(mStartAngle)
            .arg(mStyle);

    QPixmap pixmap;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int side = qMin(width(), height());

    if(!QPixmapCache::find(key, &pixmap)) {
        pixmap = generatePixmap(side);
        QPixmapCache::insert(key, pixmap);
    }

    painter.translate(width() / 2 - side / 2, height() / 2 - side / 2);

    painter.drawPixmap(0, 0, side, side, pixmap);
}

QSize ImprovedBusyIndicator::minimumSizeHint() const
{
    return QSize(20, 20);
}

QSize ImprovedBusyIndicator::sizeHint() const
{
    return QSize(30, 30);
}
