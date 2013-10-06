#ifndef BASICDIALOGS_H
#define BASICDIALOGS_H

#include "stdafx.h"

namespace BasicDialogs
{
bool okToDelete(QWidget *parent, const QString &title, const QString &text,
                const QString detailedText = "");
bool question(QWidget *parent, const QString &title, const QString &text,
              const QString detailedText = "");
}

#endif // BASICDIALOGS_H
