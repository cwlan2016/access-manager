#include "converters.h"

#include "constant.h"

bool validIpAddress(QString ip)
{
    QString regExpStr = "\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}"
                        "(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b";
    QRegExp regExp(regExpStr, Qt::CaseInsensitive, QRegExp::RegExp);

    return regExp.indexIn(ip) != -1;
}

QString toString(u_char *string, int str_len)
{
    return QString::fromLatin1(reinterpret_cast<char *>(string), str_len);
}

QString toString(u_long *oid, int oid_len)
{
    QString result = "";
    for (int i = 0 ; i < oid_len; ++i)
        result += "." + QString::number(oid[i]);

    return result;
}
