#include "basicdialogs.h"

namespace BasicDialogs
{
bool okToDelete(QWidget *parent, const QString &title, const QString &text,
                const QString detailedText)
{
    QScopedPointer<QMessageBox> messageBox(new QMessageBox(parent));

    if (parent)
        messageBox ->setWindowModality(Qt::WindowModal);

    messageBox->setIcon(QMessageBox::Question);
    messageBox->setWindowTitle(QString("%1 - %2")
                               .arg(qApp->applicationName(), title));
    messageBox->setText(text);

    if (!detailedText.isEmpty())
        messageBox->setDetailedText(detailedText);

    QAbstractButton *deleteButton = messageBox->addButton(QString::fromUtf8("Удалить"),
                                    QMessageBox::AcceptRole);
    messageBox->addButton(QString::fromUtf8("Не удалять"),
                          QMessageBox::RejectRole);
    messageBox->setDefaultButton(qobject_cast<QPushButton *>(deleteButton));

    messageBox->exec();

    return messageBox->clickedButton() == deleteButton;
}

bool question(QWidget *parent, const QString &title, const QString &text,
              const QString detailedText)
{
    QScopedPointer<QMessageBox> messageBox(new QMessageBox(parent));

    if (parent)
        messageBox->setWindowModality(Qt::WindowModal);

    messageBox->setIcon(QMessageBox::Question);
    messageBox->setWindowTitle(QString("%1 - %2")
                               .arg(qApp->applicationName(), title));
    messageBox->setText(text);

    if (!detailedText.isEmpty())
        messageBox->setDetailedText(detailedText);

    messageBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);


    messageBox->exec();

    return  messageBox->buttonRole(messageBox->clickedButton()) == QMessageBox::YesRole;
}
}
