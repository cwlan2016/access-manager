#include "converters.h"

#include "constant.h"

BoardType::Enum boardTypeFromBoardName(QString boardName)
{
    if (boardName.contains("ADEF", Qt::CaseInsensitive)) {
        return BoardType::AnnexA;
    } else if (boardName.contains("ADBF", Qt::CaseInsensitive)) {
        return BoardType::AnnexB;
    } else if (boardName.contains("SHEA", Qt::CaseInsensitive)) {
        return BoardType::Shdsl;
    } else {
        return BoardType::Other;
    }
}

bool validIpAddress(QString ip)
{
    QString regExpStr = "\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}"
                        "(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b";
    QRegExp regExp(regExpStr, Qt::CaseInsensitive, QRegExp::RegExp);

    return regExp.indexIn(ip) != -1;
}

QString typeLineString(int type)
{
    if (type == 2) {
        return "Fast";
    } else if (type == 3) {
        return "Interleave";
    } else {
        return "другой";
    }
}

QString codingString(int coding)
{
    if (coding == 2) {
        return "dmt";
    } else if (coding == 3) {
        return "cap";
    } else if (coding == 4) {
        return "cam";
    } else {
        return "другой";
    }
}

QString toString(u_char *string, int str_len)
{
    return QString::fromLatin1(reinterpret_cast<char *>(string), str_len);
}
