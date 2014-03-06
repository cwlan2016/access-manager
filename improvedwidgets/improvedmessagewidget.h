/* This file is part of the KDE libraries
 *
 * with modification for show detailed message text
 *
 * Copyright (c) 2011 Aurélien Gâteau <agateau@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QFrame>

class ImprovedMessageWidgetPrivate;

/**
 * @short A widget to provide feedback or propose opportunistic interactions.
 *
 * ImprovedMessageWidget can be used to provide inline positive or negative
 * feedback, or to implement opportunistic interactions.
 *
 * As a feedback widget, ImprovedMessageWidget provides a less intrusive alternative
 * to "OK Only" message boxes. If you do not need the modalness of KMessageBox,
 * consider using ImprovedMessageWidget instead.
 *
 * <b>Negative feedback</b>
 *
 * The ImprovedMessageWidget can be used as a secondary indicator of failure: the
 * first indicator is usually the fact the action the user expected to happen
 * did not happen.
 *
 * Example: User fills a form, clicks "Submit".
 *
 * @li Expected feedback: form closes
 * @li First indicator of failure: form stays there
 * @li Second indicator of failure: a ImprovedMessageWidget appears on top of the
 * form, explaining the error condition
 *
 * When used to provide negative feedback, ImprovedMessageWidget should be placed
 * close to its context. In the case of a form, it should appear on top of the
 * form entries.
 *
 * ImprovedMessageWidget should get inserted in the existing layout. Space should not
 * be reserved for it, otherwise it becomes "dead space", ignored by the user.
 * ImprovedMessageWidget should also not appear as an overlay to prevent blocking
 * access to elements the user needs to interact with to fix the failure.
 *
 * <b>Positive feedback</b>
 *
 * ImprovedMessageWidget can be used for positive feedback but it shouldn't be
 * overused. It is often enough to provide feedback by simply showing the
 * results of an action.
 *
 * Examples of acceptable uses:
 *
 * @li Confirm success of "critical" transactions
 * @li Indicate completion of background tasks
 *
 * Example of inadapted uses:
 *
 * @li Indicate successful saving of a file
 * @li Indicate a file has been successfully removed
 *
 * <b>Opportunistic interaction</b>
 *
 * Opportunistic interaction is the situation where the application suggests to
 * the user an action he could be interested in perform, either based on an
 * action the user just triggered or an event which the application noticed.
 *
 * Example of acceptable uses:
 *
 * @li A browser can propose remembering a recently entered password
 * @li A music collection can propose ripping a CD which just got inserted
 * @li A chat application may notify the user a "special friend" just connected
 *
 * @author Aurélien Gâteau <agateau@kde.org>
 * @since 4.7
 */

class ImprovedMessageWidget : public QFrame
{
    Q_OBJECT
    Q_ENUMS(MessageType)

    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(bool wordWrap READ wordWrap WRITE setWordWrap)
    Q_PROPERTY(bool closeButtonVisible READ isCloseButtonVisible WRITE setCloseButtonVisible)
    Q_PROPERTY(MessageType messageType READ messageType WRITE setMessageType)
    Q_PROPERTY(QIcon icon READ icon WRITE setIcon)

public:
    enum MessageType {
            Positive,
            Information,
            Warning,
            Error
        };

        /**
         * Constructs a ImprovedMessageWidget with the specified parent.
         */

    explicit ImprovedMessageWidget(QWidget *parent = 0);
    explicit ImprovedMessageWidget(const QString &text, QWidget *parent = 0);

    ~ImprovedMessageWidget();

    QString text() const;

    bool wordWrap() const;

    bool isCloseButtonVisible() const;

    MessageType messageType() const;

    void addAction(QAction *action);

    void removeAction(QAction *action);

    QSize sizeHint() const;

    QSize minimumSizeHint() const;

    int heightForWidth(int width) const;

    /**
        * The icon shown on the left of the text. By default, no icon is shown.
        * @since 4.11
        */
    QIcon icon() const;

public Q_SLOTS:
    void setText(const QString &text, const QString &detailedText = "");

    void setWordWrap(bool wordWrap);

    void setCloseButtonVisible(bool visible);

    void setMessageType(ImprovedMessageWidget::MessageType type);

    /**
        * Show the widget using an animation, unless
        * KGlobalSettings::graphicsEffectLevel() does not allow simple effects.
        */
    void animatedShow();

    /**
        * Hide the widget using an animation, unless
        * KGlobalSettings::graphicsEffectLevel() does not allow simple effects.
        */
    void animatedHide();

    /**
        * Define an icon to be shown on the left of the text
        * @since 4.11
        */
    void setIcon(const QIcon &icon);

Q_SIGNALS:
    /**
        * This signal is emitted when the user clicks a link in the text label.
        * The URL referred to by the href anchor is passed in contents.
        * @param contents text of the href anchor
        * @see QLabel::linkActivated()
        * @since 4.10
        */
    void linkActivated(const QString &contents);

    /**
        * This signal is emitted when the user hovers over a link in the text label.
        * The URL referred to by the href anchor is passed in contents.
        * @param contents text of the href anchor
        * @see QLabel::linkHovered()
        * @since 4.11
        */
    void linkHovered(const QString &contents);

protected:
    void paintEvent(QPaintEvent *event);

    bool event(QEvent *event);

    void resizeEvent(QResizeEvent *event);

    void showEvent(QShowEvent *event);

private:
    ImprovedMessageWidgetPrivate *const d;
    friend class ImprovedMessageWidgetPrivate;

    Q_PRIVATE_SLOT(d, void slotTimeLineChanged(qreal))
    Q_PRIVATE_SLOT(d, void slotTimeLineFinished())
    Q_PRIVATE_SLOT(d, void detailedButtonPressed())
};

//---------------------------------------------------------------------
// ImprovedMessageWidgetPrivate
//---------------------------------------------------------------------
class QLabel;
class QTextEdit;
class QToolButton;
class QTimeLine;

#include <QIcon>

class ImprovedMessageWidgetPrivate
{
public:
    void init(ImprovedMessageWidget *);

    ImprovedMessageWidget *q;
    QFrame *content;
    QLabel *iconLabel;
    QLabel *textLabel;
    QTextEdit *detailedTextEdit;
    QFrame *detailedTextFrame;
    QToolButton *closeButton;
    QToolButton *detailedButton;
    QTimeLine *timeLine;
    QIcon icon;

    ImprovedMessageWidget::MessageType messageType;
    bool wordWrap;
    QList<QToolButton *> buttons;
    QPixmap contentSnapShot;

    void createLayout();
    void updateSnapShot();
    void updateLayout();
    void slotTimeLineChanged(qreal);
    void slotTimeLineFinished();
    void detailedButtonPressed();
    void showDetailedText();
    void hideDetailedText();

    int bestContentHeight() const;
};

#endif
