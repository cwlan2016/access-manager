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

QString decMacAddressToHex(oid *macAddressOid, int length)
{
    QString tempStr = "";

    for (int i = 14; i < length; ++i) {
        if (i != 14) {
            tempStr = QString("%1-%2").arg(tempStr).arg(macAddressOid[i], 2, 16, QLatin1Char('0'));
        } else {
            tempStr = QString("%1").arg(macAddressOid[i], 2, 16, QLatin1Char('0'));
        }
    }

    return tempStr.toUpper();
}

bool validIpAddress(QString ip)
{
    QString regExpStr = "\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}"
                        "(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b";
    QRegExp regExp(regExpStr, Qt::CaseInsensitive, QRegExp::RegExp);

    return regExp.indexIn(ip) != -1;
}

QBitArray ucharToQBitArray(DeviceModel::Enum deviceModel, uchar *str)
{
    int countBits = 0;

    if ((deviceModel == DeviceModel::DES3526)
            || (deviceModel == DeviceModel::DES3528)) {
        countBits = 32;
    } else if (deviceModel == DeviceModel::DES3550) {
        countBits = 56;
    }

    QBitArray array(countBits, false);

    int firstBit = 0;
    int countBytes = countBits / 8;

    for (int i = 0; i < countBytes; ++i) {
        quint8 byteElement = str[i];

        if (byteElement == 0) {
            //так как массив уже инициализирован нулями
            //если текущий байт равен 0, итерация просто пропускается
            firstBit += 8;
            continue;
        }

        for (int bits = 0; bits < 8; ++bits) {
            array[7 - bits + firstBit] = byteElement % 2 == 1 ? true : false;

            byteElement /= 2;
        }

        firstBit += 8;
    }

    return array;
}

QString qbitArrayToHexString(DeviceModel::Enum deviceModel, const QBitArray &array, bool ismv)
{
    QString tempStr = "";

    quint8 value = 0;

    int size = array.size();
    for (int i = 0; i < size; ++i) {
        value <<= 1;
        value += (quint8)array.at(i);

        if ((i + 1) % 8 == 0) { //проверка на последний bit в текущем байте
            tempStr.push_back(QString(" %1").arg(value, 2, 16, QLatin1Char('0')));
            value = 0;
        }
    }

    if ((deviceModel == DeviceModel::DES3528) && ismv) {
        // Значение MembersISMV в DES-3528 имеет размер 64 байта
        // Дополняем HEX-строку до 64 байт.
        for (int i = 0; i < 60; ++i)
            tempStr.push_back(QString(" 00"));
    }

    return tempStr.trimmed().toUpper();
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

QString profileExtName(DeviceModel::Enum deviceModel, QString profile)
{
    if (deviceModel == DeviceModel::MA5600) {
        if ((profile == AdslProfileDslamName::MA5600::AdslFast)
                || (profile == AdslProfileDslamName::MA5600::AdslInterleave)
                || (profile == "DEFVAL")) {
            return QString::fromUtf8("ADSL (%1)").arg(profile);
        } else if (profile == AdslProfileDslamName::MA5600::Adsl2Interleave) {
            return QString::fromUtf8("ADSL 2+ (%1)").arg(profile);
        } else if (profile.contains("g.lite", Qt::CaseInsensitive)
                   || profile.contains("g-lite", Qt::CaseInsensitive)) {
            return QString::fromUtf8("G.Lite (%1)").arg(profile);
        } else {
            return QString::fromUtf8("(%1)").arg(profile);
        }
    } else if (deviceModel == DeviceModel::MA5300) {
        if ((profile == AdslProfileDslamName::MA5300::AdslFast)
                || (profile == AdslProfileDslamName::MA5300::AdslInterleave)) {
            return QString::fromUtf8("ADSL (%1)").arg(profile);
        } else if (profile == AdslProfileDslamName::MA5300::Adsl2Interleave) {
            return QString::fromUtf8("ADSL 2+ (%1)").arg(profile);
        } else if (profile.contains("g.lite", Qt::CaseInsensitive)
                   || profile.contains("g-lite", Qt::CaseInsensitive)) {
            return QString::fromUtf8("G.Lite (%1)").arg(profile);
        } else {
            return QString::fromUtf8("(%1)").arg(profile);
        }
    } else if ((deviceModel == DeviceModel::MXA64)
               || (deviceModel == DeviceModel::MXA32)) {
        if (profile == AdslProfileDslamName::MXA::AdslInterleave) {
            return "ADSL";
        } else if (profile == AdslProfileDslamName::MXA::AdslGLite) {
            return "G.Lite";
        } else if (profile == AdslProfileDslamName::MXA::Adsl2Interleave) {
            return "ADSL 2+";
        } else {
            return "Other";
        }
    } else {
        return QString();
    }
}

QString displayNameProfileToDslamName(DeviceModel::Enum deviceModel, QString profile)
{
    if (deviceModel == DeviceModel::MA5600) {
        if (profile == AdslProfileDisplayName::AdslFast) {
            return AdslProfileDslamName::MA5600::AdslFast;
        } else if (profile == AdslProfileDisplayName::AdslInterleave) {
            return AdslProfileDslamName::MA5600::AdslInterleave;
        } else if (profile == AdslProfileDisplayName::Adsl2Interleave) {
            return AdslProfileDslamName::MA5600::Adsl2Interleave;
        } else if (profile == AdslProfileDisplayName::AdslGLite) {
            return AdslProfileDslamName::MA5600::AdslGLite;
        } else if (profile == AdslProfileDisplayName::AdslFast8Mb) {
            return AdslProfileDslamName::MA5600::AdslFast8Mb;
        } else if (profile == AdslProfileDisplayName::Adsl2Fast10Mb) {
            return AdslProfileDslamName::MA5600::Adsl2Fast10Mb;
        } else if (profile == AdslProfileDisplayName::Adsl2Fast14Mb) {
            return AdslProfileDslamName::MA5600::Adsl2Fast14Mb;
        } else if (profile == AdslProfileDisplayName::Adsl2Fast18Mb) {
            return AdslProfileDslamName::MA5600::Adsl2Fast18Mb;
        }
    } else if (deviceModel == DeviceModel::MA5300) {
        if (profile == AdslProfileDisplayName::AdslFast) {
            return AdslProfileDslamName::MA5300::AdslFast;
        } else if (profile == AdslProfileDisplayName::AdslInterleave) {
            return AdslProfileDslamName::MA5300::AdslInterleave;
        } else if (profile == AdslProfileDisplayName::Adsl2Interleave) {
            return AdslProfileDslamName::MA5300::Adsl2Interleave;
        }
    } else if ((deviceModel == DeviceModel::MXA64)
               || (deviceModel == DeviceModel::MXA32)) {
        if (profile == AdslProfileDisplayName::AdslInterleave) {
            return AdslProfileDslamName::MXA::AdslInterleave;
        } else if (profile == AdslProfileDisplayName::Adsl2Interleave) {
            return AdslProfileDslamName::MXA::Adsl2Interleave;
        } else if (profile == AdslProfileDisplayName::AdslGLite) {
            return AdslProfileDslamName::MXA::AdslGLite;
        }
    }

    return QString();

}

QString toString(u_char *string, int str_len)
{
    return QString::fromLatin1(reinterpret_cast<char *>(string), str_len);
}
