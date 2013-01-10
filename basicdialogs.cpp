#include "basicdialogs.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <memory>

namespace BasicDialogs
{
bool okToDelete(QWidget *parent, const QString &title, const QString &text, const QString &detailedText)
{
    QScopedPointer<QMessageBox> messageBox(new QMessageBox(parent));

    if (parent)
        messageBox ->setWindowModality(Qt::WindowModal);

    messageBox->setIcon(QMessageBox::Question);
    messageBox->setWindowTitle(QString("%1 - %2").arg(qApp->applicationName(), title));
    messageBox->setText(text);

    if (!detailedText.isEmpty())
        messageBox->setDetailedText(detailedText);

    QAbstractButton *deleteButton = messageBox->addButton(QString::fromUtf8("Удалить"),
                                    QMessageBox::AcceptRole);
    messageBox->addButton(QString::fromUtf8("Не удалять"), QMessageBox::RejectRole);
    messageBox->setDefaultButton(qobject_cast<QPushButton *>(deleteButton));

    messageBox->exec();

    return messageBox->clickedButton() == deleteButton;
}

void error(QWidget *parent, const QString &title, const QString &text, const QString &detailedText)
{
    QScopedPointer<QMessageBox> messageBox(new QMessageBox(parent));

    if (parent) {
        messageBox->setWindowModality(Qt::WindowModal);
    }

    messageBox->setIcon(QMessageBox::Critical);
    messageBox->setWindowTitle(QString("%1 - %2").arg(qApp->applicationName(), title));
    messageBox->setText(text);

    if (!detailedText.isEmpty()) {
        messageBox->setDetailedText(detailedText);
    }

    messageBox->addButton(QMessageBox::Ok);

    messageBox->exec();
}

void information(QWidget *parent, const QString &title, const QString &text,
                 const QString &detailedText)
{
    QScopedPointer<QMessageBox> messageBox(new QMessageBox(parent));

    if (parent)
        messageBox->setWindowModality(Qt::WindowModal);

    messageBox->setIcon(QMessageBox::Information);
    messageBox->setWindowTitle(QString("%1 - %2").arg(qApp->applicationName(), title));
    messageBox->setText(text);

    if (!detailedText.isEmpty())
        messageBox->setDetailedText(detailedText);

    messageBox->addButton(QMessageBox::Ok);

    messageBox->exec();
}

void warning(QWidget *parent, const QString &title, const QString &text,
             const QString &detailedText)
{
    QScopedPointer<QMessageBox> messageBox(new QMessageBox(parent));

    if (parent)
        messageBox->setWindowModality(Qt::WindowModal);

    messageBox->setIcon(QMessageBox::Warning);
    messageBox->setWindowTitle(QString("%1 - %2").arg(qApp->applicationName(), title));
    messageBox->setText(text);

    if (!detailedText.isEmpty())
        messageBox->setInformativeText(detailedText);

    messageBox->addButton(QMessageBox::Ok);

    messageBox->exec();
}

bool question(QWidget *parent, const QString &title, const QString &text,
              const QString &detailedText)
{
    QScopedPointer<QMessageBox> messageBox(new QMessageBox(parent));

    if (parent)
        messageBox->setWindowModality(Qt::WindowModal);

    messageBox->setIcon(QMessageBox::Question);
    messageBox->setWindowTitle(QString("%1 - %2").arg(qApp->applicationName(), title));
    messageBox->setText(text);

    if (!detailedText.isEmpty())
        messageBox->setDetailedText(detailedText);

    messageBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);


    messageBox->exec();

    return  messageBox->result() == QDialog::Accepted;
}
}
