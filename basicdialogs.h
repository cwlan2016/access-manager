#ifndef BASICDIALOGS_H
#define BASICDIALOGS_H

#include "QtWidgets/QWidget"

namespace BasicDialogs
{
bool okToDelete(QWidget *parent, const QString &title, const QString &text, const QString &detailedText = "");
void error(QWidget *parent, const QString &title, const QString &text, const QString &detailedText = "");
void warning(QWidget *parent, const QString &title, const QString &text, const QString &detailedText = "");
void information(QWidget *parent, const QString &title, const QString &text, const QString &detailedText = "");
bool question(QWidget *parent, const QString &title, const QString &text, const QString &detailedText = "");
}

#endif // BASICDIALOGS_H
