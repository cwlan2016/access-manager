/* Original component from
 *
 * https://github.com/anjinkristou/Qt-busy-indicator
 *
 * Added property for QtDesigner
 *
 */

#ifndef IMPROVEDBUSYINDICATOR_H
#define IMPROVEDBUSYINDICATOR_H

#include <QTimer>
#include <QWidget>

class ImprovedBusyIndicator : public QWidget
{
    Q_OBJECT
    Q_ENUMS(IndicatorStyle)

    Q_PROPERTY(IndicatorStyle indicatorStyle READ indicatorStyle WRITE setIndicatorStyle)
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(int interval READ interval WRITE setInterval)

public:
    enum IndicatorStyle {
        StyleRect,
        StyleEllipse,
        StyleArc
    };

    explicit ImprovedBusyIndicator(QWidget *parent = 0);

    void paintEvent(QPaintEvent *);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void setIndicatorStyle(IndicatorStyle);
    void setColor(QColor color);
    void setInterval(int interval);

    IndicatorStyle indicatorStyle() const;
    QColor color() const;
    int interval() const;

private slots:
    void rotate();

private:
    QPixmap generatePixmap(int sideLength);
    void drawRectStyle(QPainter *painter);
    void drawEllipseStyle(QPainter *painter);
    void drawArcStyle(QPainter *painter);

    QTimer mTimer;
    int mStartAngle;

    IndicatorStyle mStyle;

    QColor mFillColor;
};

#endif // IMPROVEDBUSYINDICATOR_H
