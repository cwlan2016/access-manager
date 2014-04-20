#include "converters.h"

#include "constant.h"

bool validIpAddress(QString ip)
{
    QString regExpStr = "\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}"
                        "(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b";
    QRegExp regExp(regExpStr, Qt::CaseInsensitive, QRegExp::RegExp);

    return regExp.indexIn(ip) != -1;
}

QString toString(quint8 *cstring, int strLen)
{
    return QString::fromLatin1(reinterpret_cast<char *>(cstring), strLen);
}

QString toString(ulong *oid, int oidLen)
{
    QString result = "";
    for (int i = 0 ; i < oidLen; ++i)
        result += '.' + QString::number(oid[i]);

    return result;
}
