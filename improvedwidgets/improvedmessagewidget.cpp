#include "improvedmessagewidget.h"

#include <QEvent>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QShowEvent>
#include <QScrollBar>
#include <QTimeLine>
#include <QTextEdit>
#include <QToolButton>
#include <QStyle>
#include <QAction>

//---------------------------------------------------------------------
// ImprovedMessageWidget
//---------------------------------------------------------------------
ImprovedMessageWidget::ImprovedMessageWidget(QWidget *parent)
    : QFrame(parent),
      d(new ImprovedMessageWidgetPrivate)
{
    d->init(this);
}

ImprovedMessageWidget::ImprovedMessageWidget(const QString &text, QWidget *parent)
    : QFrame(parent),
      d(new ImprovedMessageWidgetPrivate)
{
    d->init(this);
    setText(text);
}

ImprovedMessageWidget::~ImprovedMessageWidget()
{
    delete d;
}

QString ImprovedMessageWidget::text() const
{
    return d->textLabel->text();
}

void ImprovedMessageWidget::setText(const QString &text, const QString &detailedText)
{
    d->textLabel->setText(text);
    d->detailedTextEdit->setText(detailedText);

    if (!detailedText.isEmpty()) {
        d->detailedButton->show();
    } else {
        d->detailedButton->hide();
        d->hideDetailedText();
    }

    updateGeometry();
}

ImprovedMessageWidget::MessageType ImprovedMessageWidget::messageType() const
{
    return d->messageType;
}

void ImprovedMessageWidget::setMessageType(ImprovedMessageWidget::MessageType type)
{
    d->messageType = type;
    QColor bg0, bg1, bg2, border, fg;

    switch (type) {
    case Positive:
        bg1 = QColor("#63DA96"); // nice green
        fg = QColor(Qt::white);
        setIcon(QIcon(":/images/positive.png"));
        break;

    case Information:
        bg1 = QColor("#A1C2F0"); // nice blue
        fg = QColor(Qt::black);
        setIcon(QIcon(":/images/information.png"));
        break;

    case Warning:
        bg1 = QColor("#E1C460"); // nice yellow
        fg = QColor(Qt::black);
        setIcon(QIcon(":/images/warning.png"));
        break;

    case Error:
        bg1 = QColor("#E4B5B7"); // nice red.
        fg = QColor(Qt::black);
        setIcon(QIcon(":/images/error.png"));
        break;
    }

    // Colors
    bg0 = bg1.lighter(110);
    bg2 = bg1.darker(110);
    border = bg2.darker(110);
    d->content->setStyleSheet(
        QString(".QFrame {"
            "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            "    stop: 0 %1,"
            "    stop: 0.1 %2,"
            "    stop: 1.0 %3);"
            "border-radius: 5px;"
            "border: 1px solid %4;"
            "margin: %5px;"
            "}"
            ".QLabel { color: %6; }"
            ".QTextEdit { color: %6; background-color: rgba(0, 0, 0, 0); }"
            ".QScrollBar {"
            "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
            "    stop: 0 %1,"
            "    stop: 0.1 %2,"
            "    stop: 1.0 %3);"
            "}"
        ).arg(bg0.name())
        .arg(bg1.name())
        .arg(bg2.name())
        .arg(border.name())
        /*
        DefaultFrameWidth returns the size of the external margin + border width.
        We know our border is 1px, so we subtract this from the frame
        normal QStyle FrameWidth to get our margin
        */

        .arg(style()->pixelMetric(QStyle::PM_DefaultFrameWidth, 0, this) -1)
        .arg(fg.name())
    );
}

QSize ImprovedMessageWidget::sizeHint() const
{
    ensurePolished();
    return d->content->sizeHint();
}

QSize ImprovedMessageWidget::minimumSizeHint() const
{
    ensurePolished();
    return d->content->minimumSizeHint();
}

bool ImprovedMessageWidget::event(QEvent* event)
{
    if (event->type() == QEvent::Polish && !d->content->layout())
        d->createLayout();

    return QFrame::event(event);
}

void ImprovedMessageWidget::resizeEvent(QResizeEvent* event)
{
    QFrame::resizeEvent(event);

    if (d->timeLine->state() == QTimeLine::NotRunning)
        d->content->resize(width(), d->bestContentHeight());
}

int ImprovedMessageWidget::heightForWidth(int width) const
{
    ensurePolished();
    return d->content->heightForWidth(width);
}

void ImprovedMessageWidget::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    if (d->timeLine->state() == QTimeLine::Running) {
        QPainter painter(this);
        painter.setOpacity(d->timeLine->currentValue() * d->timeLine->currentValue());
        painter.drawPixmap(0, 0, d->contentSnapShot);
    }
}

void ImprovedMessageWidget::showEvent(QShowEvent *event)
{
    // Keep this method here to avoid breaking binary compatibility:
    // QFrame::showEvent() used to be reimplemented.
    QFrame::showEvent(event);
}

bool ImprovedMessageWidget::wordWrap() const
{
    return d->wordWrap;
}

void ImprovedMessageWidget::setWordWrap(bool wordWrap)
{
    d->wordWrap = wordWrap;
    d->textLabel->setWordWrap(wordWrap);
    QSizePolicy policy = sizePolicy();
    policy.setHeightForWidth(wordWrap);
    setSizePolicy(policy);
    d->updateLayout();

    // Without this, when user does wordWrap -> !wordWrap -> wordWrap, a minimum
    // height is set, causing the widget to be too high.
    // Mostly visible in test programs.
    if (wordWrap)
        setMinimumHeight(0);
}

bool ImprovedMessageWidget::isCloseButtonVisible() const
{
    return d->closeButton->isVisible();
}

void ImprovedMessageWidget::setCloseButtonVisible(bool show)
{
    d->closeButton->setVisible(show);
    updateGeometry();
}

void ImprovedMessageWidget::addAction(QAction* action)
{
    QFrame::addAction(action);
    d->updateLayout();
}

void ImprovedMessageWidget::removeAction(QAction* action)
{
    QFrame::removeAction(action);
    d->updateLayout();
}

void ImprovedMessageWidget::animatedShow()
{
    if (isVisible())
        return;

    QFrame::show();
    setFixedHeight(0);
    int wantedHeight = d->bestContentHeight();
    d->content->setGeometry(0, -wantedHeight, width(), wantedHeight);

    d->updateSnapShot();

    d->timeLine->setDirection(QTimeLine::Forward);

    if (d->timeLine->state() == QTimeLine::NotRunning)
        d->timeLine->start();
}

void ImprovedMessageWidget::animatedHide()
{
    if (!isVisible()) {
        hide();
        return;
    }

    d->content->move(0, -d->content->height());
    d->updateSnapShot();

    d->timeLine->setDirection(QTimeLine::Backward);

    if (d->timeLine->state() == QTimeLine::NotRunning)
        d->timeLine->start();
}

QIcon ImprovedMessageWidget::icon() const
{
    return d->icon;
}

void ImprovedMessageWidget::setIcon(const QIcon& icon)
{
    d->icon = icon;

    if (d->icon.isNull()) {
        d->iconLabel->hide();
    } else {
        d->iconLabel->setPixmap(d->icon.pixmap(QSize(16,16)));
        d->iconLabel->show();
    }
}

//---------------------------------------------------------------------
// ImprovedMessageWidgetPrivate
//---------------------------------------------------------------------
void ImprovedMessageWidgetPrivate::init(ImprovedMessageWidget* q_ptr)
{
    q = q_ptr;

    q->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    timeLine = new QTimeLine(500, q);
    QObject::connect(timeLine, SIGNAL(valueChanged(qreal)), q, SLOT(slotTimeLineChanged(qreal)));
    QObject::connect(timeLine, SIGNAL(finished()), q, SLOT(slotTimeLineFinished()));

    content = new QFrame(q);
    content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    wordWrap = false;

    iconLabel = new QLabel(content);
    iconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    iconLabel->hide();

    textLabel = new QLabel(content);
    textLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    textLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    QObject::connect(textLabel, SIGNAL(linkActivated(const QString&)), q, SIGNAL(linkActivated(const QString&)));
    QObject::connect(textLabel, SIGNAL(linkHovered(const QString&)), q, SIGNAL(linkHovered(const QString&)));

    detailedTextFrame = new QFrame(content);
    detailedTextFrame->hide();

    detailedTextEdit = new QTextEdit(detailedTextFrame);
    detailedTextEdit->setReadOnly(true);
    detailedTextEdit->setMaximumHeight(60);

    QVBoxLayout *detailedFrameLayout = new QVBoxLayout(detailedTextFrame);
    detailedFrameLayout->addWidget(detailedTextEdit);

    QAction *closeAction = new QAction(QIcon(":/images/close.png"), QObject::trUtf8("Закрыть"), q);
    q->connect(closeAction, SIGNAL(triggered(bool)), q, SLOT(animatedHide()));

    closeButton = new QToolButton(content);
    closeButton->setAutoRaise(true);
    closeButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    closeButton->setDefaultAction(closeAction);

    QAction *detailedAction = new QAction(QIcon(":/images/go-down.png"), QObject::trUtf8("Показать подробности"), q);
    q->connect(detailedAction, SIGNAL(triggered(bool)), q, SLOT(detailedButtonPressed()));

    detailedButton = new QToolButton(content);
    detailedButton->setAutoRaise(true);
    detailedButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    detailedButton->setDefaultAction(detailedAction);
    detailedButton->hide();

    q->setMessageType(ImprovedMessageWidget::Information);
}

void ImprovedMessageWidgetPrivate::createLayout()
{
    delete content->layout();

    content->resize(q->size());

    qDeleteAll(buttons);
    buttons.clear();

    Q_FOREACH(QAction *action, q->actions()) {
        QToolButton *button = new QToolButton(content);
        button->setDefaultAction(action);
        button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        buttons.append(button);
    }

    // AutoRaise reduces visual clutter, but we don't want to turn it on if
    // there are other buttons, otherwise the close button will look different
    // from the others.
    closeButton->setAutoRaise(buttons.isEmpty());
    detailedButton->setAutoRaise(buttons.isEmpty());

    if (wordWrap) {
        QGridLayout *layout = new QGridLayout(content);
        layout->setMargin(2);
        // Set alignment to make sure icon does not move down if text wraps
        layout->addWidget(iconLabel, 0, 0, 1, 1, Qt::AlignHCenter | Qt::AlignTop);
        layout->addWidget(textLabel, 0, 1);

        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->setMargin(2);
        buttonLayout->addStretch();
        Q_FOREACH(QToolButton* button, buttons) {
            // For some reason, calling show() is necessary if wordwrap is true,
            // otherwise the buttons do not show up. It is not needed if
            // wordwrap is false.
            button->show();
            buttonLayout->addWidget(button);
        }

        buttonLayout->addWidget(detailedButton);
        buttonLayout->addWidget(closeButton);
        layout->addItem(buttonLayout, 0, 2);

        layout->addWidget(detailedTextFrame, 1, 0, 1, 3);

    } else {
        QVBoxLayout *mainLayout = new QVBoxLayout(content);
        mainLayout->setMargin(2);

        QHBoxLayout *layout = new QHBoxLayout();
        layout->setMargin(2);
        layout->addWidget(iconLabel);
        layout->addWidget(textLabel);

        Q_FOREACH(QToolButton *button, buttons) {
            layout->addWidget(button);
        }

        layout->addWidget(detailedButton);
        layout->addWidget(closeButton);

        mainLayout->addItem(layout);
        mainLayout->addWidget(detailedTextFrame);
    };

    if (q->isVisible())
        q->setFixedHeight(content->sizeHint().height());

    q->updateGeometry();
}

void ImprovedMessageWidgetPrivate::updateLayout()
{
    if (content->layout()) {
        createLayout();
    }
}

void ImprovedMessageWidgetPrivate::updateSnapShot()
{
    // Attention: updateSnapShot calls QWidget::render(), which causes the whole
    // window layouts to be activated. Calling this method from resizeEvent()
    // can lead to infinite recursion, see:
    // https://bugs.kde.org/show_bug.cgi?id=311336
    contentSnapShot = QPixmap(content->size());
    contentSnapShot.fill(Qt::transparent);
    content->render(&contentSnapShot, QPoint(), QRegion(), QWidget::DrawChildren);
}

void ImprovedMessageWidgetPrivate::slotTimeLineChanged(qreal value)
{
    q->setFixedHeight(qMin(value * 2, qreal(1.0)) * content->height());
    q->update();
}

void ImprovedMessageWidgetPrivate::slotTimeLineFinished()
{
    if (timeLine->direction() == QTimeLine::Forward) {
        // Show
        // We set the whole geometry here, because it may be wrong if a
        // ImprovedMessageWidget is shown right when the toplevel window is created.
        content->setGeometry(0, 0, q->width(), bestContentHeight());
    } else {
        // Hide
        q->hide();
    }
}

void ImprovedMessageWidgetPrivate::detailedButtonPressed()
{
    if (detailedTextFrame->isVisible()) {
        hideDetailedText();
    } else {
        showDetailedText();
    }

    updateLayout();
}

void ImprovedMessageWidgetPrivate::showDetailedText()
{
    detailedTextFrame->show();
    int minimum = detailedTextEdit->verticalScrollBar()->minimum();
    detailedTextEdit->verticalScrollBar()->setValue(minimum);
    detailedButton->defaultAction()->setIcon(QIcon(":/images/go-up.png"));
    detailedButton->defaultAction()->setText(QObject::trUtf8("Скрыть подробности"));
}

void ImprovedMessageWidgetPrivate::hideDetailedText()
{
    detailedTextFrame->hide();
    detailedButton->defaultAction()->setIcon(QIcon(":/images/go-down.png"));
    detailedButton->defaultAction()->setText(QObject::trUtf8("Показать подробности"));
}

int ImprovedMessageWidgetPrivate::bestContentHeight() const
{
    int height = content->heightForWidth(q->width());

    if (height == -1)
        height = content->sizeHint().height();

    return height;
}
