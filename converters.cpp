#include "converters.h"

int snmpInterfaceNumber(DeviceModel model, int slot, int port)
{
    if (model == DeviceModel::MA5600)
    {
        return 201326592 + 8192 * slot + 64 * port;
    }
    else if (model == DeviceModel::MA5300)
    {
        return 201326592 + 65536 * slot + 64 * port;
    }
    else if ((model == DeviceModel::MXA64)
             || (model == DeviceModel::MXA32))
    {
        return port + 1;
    }
    else
    {
        return 0;
    }
}

QString DslamStatePortString(long state)
{
    if (state == 1)
        return "Up";
    else if ((state == 2) || (state == 6))
        return "Down";
    else if ((state == 5) || (state == 65536))
        return "Activating";
    else if (state == 65548)
        return "Defective";
    else
        return "Other";
}

QString SwitchStatePortString(long state)
{
    if (state == 1)
        return "Up";
    else if (state == 2)
        return "Down";
    else
        return "Other";
}

QString SpeedDuplexString(DeviceModel deviceModel, long speedDuplex)
{
    if ((deviceModel == DeviceModel::DES3526)
            || (deviceModel == DeviceModel::DES3550))
    {
        if (speedDuplex == 2)
            return "Auto";
        else if (speedDuplex == 6)
            return "100Mbps/Full";
        else if (speedDuplex == 5)
            return "100Mbps/Half";
        else if (speedDuplex == 8)
            return "1Gbps/Full";
        else if (speedDuplex == 4)
            return "10Mbps/Full";
        else if (speedDuplex == 3)
            return "10Mbps/Half";
        else if (speedDuplex == 7)
            return "1Gbps/Half";
        else
            return "Unknown";
    }
    else if (deviceModel == DeviceModel::DES3528)
    {
        if (speedDuplex == 0)
            return "Down";
        else if (speedDuplex == 6)
            return "100Mbps/Full";
        else if (speedDuplex == 8)
            return "100Mbps/Half";
        else if (speedDuplex == 10)
            return "1Gbps/Full";
        else if (speedDuplex == 2)
            return "10Mbps/Full";
        else if (speedDuplex == 4)
            return "10Mbps/Half";
        else if (speedDuplex == 12)
            return "1Gbps/Half";
        else
            return "Unknown";
    }
    else
        return "Unknown";
}

DeviceModel DeviceModelFromString(QString deviceModel)
{
    if (deviceModel == DeviceModelName[(short)DeviceModel::DES3526])
        return DeviceModel::DES3526;
    else if (deviceModel == DeviceModelName[(short)DeviceModel::MA5600])
        return DeviceModel::MA5600;
    else if (deviceModel == DeviceModelName[(short)DeviceModel::LTP8X])
        return DeviceModel::LTP8X;
    else if (deviceModel == DeviceModelName[(short)DeviceModel::LTE8ST])
        return DeviceModel::LTE8ST;
    else if (deviceModel == DeviceModelName[(short)DeviceModel::DES3528])
        return DeviceModel::DES3528;
    else if (deviceModel == DeviceModelName[(short)DeviceModel::MA5300])
        return DeviceModel::MA5300;
    else if (deviceModel == DeviceModelName[(short)DeviceModel::MXA64])
        return DeviceModel::MXA64;
    else if (deviceModel == DeviceModelName[(short)DeviceModel::DES3552])
        return DeviceModel::DES3552;
    else if (deviceModel == DeviceModelName[(short)DeviceModel::MXA32])
        return DeviceModel::MXA32;
    else if (deviceModel == DeviceModelName[(short)DeviceModel::MA5616])
        return DeviceModel::MA5616;
    else if (deviceModel == DeviceModelName[(short)DeviceModel::DES3550])
        return DeviceModel::DES3550;
    else
        return DeviceModel::Other;
}

DeviceType DeviceTypeFromString(QString deviceType)
{
    if (deviceType == DeviceTypeName[(short)DeviceType::Switch])
        return DeviceType::Switch;
    else if (deviceType == DeviceTypeName[(short)DeviceType::Dslam])
        return DeviceType::Dslam;
    else if (deviceType == DeviceTypeName[(short)DeviceType::Olt])
        return DeviceType::Olt;
    else
        return DeviceType::Other;
}

DeviceType DeviceTypeFromDeviceModel(DeviceModel deviceModel)
{
    switch(deviceModel)
    {
    case DeviceModel::DES3526:
    case DeviceModel::DES3528:
    case DeviceModel::DES3550:
    case DeviceModel::DES3552:
        return DeviceType::Switch;
    case DeviceModel::MA5600:
    case DeviceModel::MA5300:
    case DeviceModel::MA5616:
    case DeviceModel::MXA32:
    case DeviceModel::MXA64:
        return DeviceType::Dslam;
    case DeviceModel::LTP8X:
    case DeviceModel::LTE8ST:
        return DeviceType::Olt;
    case DeviceModel::Other:
        return DeviceType::Other;
    }

    return DeviceType::Other;
}

BoardType BoardTypeFromString(QString type)
{
    if (type == BoardTypeName[(short)BoardType::AnnexA])
        return BoardType::AnnexA;
    else if (type == BoardTypeName[(short)BoardType::AnnexB])
        return BoardType::AnnexB;
    else if (type == BoardTypeName[(short)BoardType::Shdsl])
        return BoardType::Shdsl;
    else
        return BoardType::Other;
}

BoardType BoardTypeFromBoardName(QString boardName)
{
    if (boardName.contains("ADEF", Qt::CaseInsensitive))
        return BoardType::AnnexA;
    else if (boardName.contains("ADBF", Qt::CaseInsensitive))
        return BoardType::AnnexB;
    else if (boardName.contains("SHEA", Qt::CaseInsensitive))
        return BoardType::Shdsl;
    else
        return BoardType::Other;
}

int CountPorts(DeviceModel deviceModel, BoardType boardType)
{
    if (deviceModel == DeviceModel::MA5600)
    {
        if ((boardType == BoardType::AnnexA)
                || (boardType == BoardType::AnnexB))
            return 64;
        else if (boardType == BoardType::Shdsl)
            return 32;
        else
            return 0;
    }
    else if (deviceModel == DeviceModel::MA5300)
    {
        if ((boardType == BoardType::AnnexA)
                || (boardType == BoardType::AnnexB))
            return 48;
        else if (boardType == BoardType::Shdsl)
            return 24;
        else
            return 0;
    }
    else if (deviceModel == DeviceModel::MXA64)
    {
        return 64;
    }
    else if (deviceModel == DeviceModel::MXA32)
    {
        return 32;
    }
    else
        return 0;
}

int CountPorts(DeviceModel deviceModel)
{
    if ((deviceModel == DeviceModel::DES3526)
            || (deviceModel == DeviceModel::DES3528))
    {
        return 26;
    }
    else if ((deviceModel == DeviceModel::DES3550)
             || (deviceModel == DeviceModel::DES3552))
    {
        return 50;
    }
    else
        return 0;
}

QString DecMacAddressToHex(oid* macAddressOid, int length)
{
    QString tempStr = "";

    for (int i = 14; i < length; i++)
    {
        if (i != 14)
            tempStr = QString("%1-%2").arg(tempStr).arg(macAddressOid[i], 2, 16, QLatin1Char('0'));
        else
            tempStr = QString("%1").arg(macAddressOid[i], 2, 16, QLatin1Char('0'));
    }

    return tempStr.toUpper();
}

bool ValidIpAddress(QString ip)
{
    QString regExpStr = "\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b";
    QRegExp regExp(regExpStr, Qt::CaseInsensitive, QRegExp::RegExp);

    return regExp.indexIn(ip) != -1;
}

QBitArray UCharToQBitArray(DeviceModel deviceModel, uchar* str)
{
    int countBits = 0;

    if ((deviceModel == DeviceModel::DES3526)
            || (deviceModel == DeviceModel::DES3528))
        countBits = 32;
    else if (deviceModel == DeviceModel::DES3550)
        countBits = 56;

    QBitArray array(countBits, false);

    int firstBit = 0;
    int countBytes = countBits / 8;

    for (int i = 0; i < countBytes; i++)
    {
        quint8 byteElement = str[i];

        if (byteElement == 0)
        {
            //так как массив уже инициализирован нулями
            //если текущий байт равен 0, итерация просто пропускается
            firstBit += 8;
            continue;
        }

        for (int bits = 0; bits < 8; bits++)
        {
            array[7 - bits + firstBit] = byteElement % 2 == 1 ? true : false;

            byteElement /= 2;
        }

        firstBit += 8;
    }

    return array;
}

QString QBitArrayToHexString(DeviceModel deviceModel, const QBitArray& array, bool ismv)
{
    QString tempStr = "";

    quint8 value = 0;

    for (int i = 0; i < array.size(); i++)
    {
        value <<= 1;
        value += (quint8)array.at(i);

        if ((i + 1) % 8 == 0)   //проверка на последний bit в текущем байте
        {
            tempStr.append(QString(" %1").arg(value, 2, 16, QLatin1Char('0')));
            value = 0;
        }
    }

    if ((deviceModel == DeviceModel::DES3528) && ismv)
    {
        // Значение MembersISMV в DES-3528 имеет размер 64 байта
        // Дополняем HEX-строку до 64 байт.
        for(int i = 0; i < 60; i++)
        {
            tempStr.append(QString(" 00"));
        }
    }

    return tempStr.trimmed().toUpper();
}

QString TypeLineString(int type)
{
    if (type == 2)
        return "Fast";
    else if (type == 3)
        return "Interleave";
    else
        return "другой";
}

QString CodingString(int coding)
{
    if (coding == 2)
        return "dmt";
    else if (coding == 3)
        return "cap";
    else if (coding == 4)
        return "cam";
    else
        return "другой";
}

QString ProfileExtName(DeviceModel deviceModel, QString profile)
{
    if (deviceModel == DeviceModel::MA5600)
    {
        if ((profile == AdslProfileDslamName::MA5600::AdslFast)
            || (profile == AdslProfileDslamName::MA5600::AdslInterleave)
            || (profile == "DEFVAL"))
        {
            return QString::fromUtf8("ADSL (%1)").arg(profile);
        }
        else if (profile == AdslProfileDslamName::MA5600::Adsl2Interleave)
        {
            return QString::fromUtf8("ADSL 2+ (%1)").arg(profile);
        }
        else if (profile.contains("g.lite", Qt::CaseInsensitive)
            || profile.contains("g-lite", Qt::CaseInsensitive))
        {
            return QString::fromUtf8("G.Lite (%1)").arg(profile);
        }
        else
        {
            return QString::fromUtf8("(%1)").arg(profile);
        }
    }
    else if (deviceModel == DeviceModel::MA5300)
    {
        if ((profile == AdslProfileDslamName::MA5300::AdslFast)
            || (profile == AdslProfileDslamName::MA5300::AdslInterleave))
        {
            return QString::fromUtf8("ADSL (%1)").arg(profile);
        }
        else if (profile == AdslProfileDslamName::MA5300::Adsl2Interleave)
        {
            return QString::fromUtf8("ADSL 2+ (%1)").arg(profile);
        }
        else if (profile.contains("g.lite", Qt::CaseInsensitive)
            || profile.contains("g-lite", Qt::CaseInsensitive))
        {
            return QString::fromUtf8("G.Lite (%1)").arg(profile);
        }
        else
        {
            return QString::fromUtf8("(%1)").arg(profile);
        }
    }
    else if ((deviceModel == DeviceModel::MXA64)
             || (deviceModel == DeviceModel::MXA32))
    {
        if (profile == AdslProfileDslamName::MXA::AdslInterleave)
        {
            return "ADSL";
        }
        else if (profile == AdslProfileDslamName::MXA::AdslGLite)
        {
            return "G.Lite";
        }
        else if (profile == AdslProfileDslamName::MXA::Adsl2Interleave)
        {
            return "ADSL 2+";
        }
        else
        {
            return "Other";
        }
    }
    else
    {
        return "";
    }
}

QString DisplayNameProfileToDslamName(DeviceModel deviceModel, QString profile)
{
    if (deviceModel == DeviceModel::MA5600)
    {
        if (profile == AdslProfileDisplayName::AdslFast)
            return AdslProfileDslamName::MA5600::AdslFast;
        else if (profile == AdslProfileDisplayName::AdslInterleave)
            return AdslProfileDslamName::MA5600::AdslInterleave;
        else if (profile == AdslProfileDisplayName::Adsl2Interleave)
            return AdslProfileDslamName::MA5600::Adsl2Interleave;
        else if (profile == AdslProfileDisplayName::AdslGLite)
            return AdslProfileDslamName::MA5600::AdslGLite;
        else if (profile == AdslProfileDisplayName::AdslFast8Mb)
            return AdslProfileDslamName::MA5600::AdslFast8Mb;
        else if (profile == AdslProfileDisplayName::Adsl2Fast10Mb)
            return AdslProfileDslamName::MA5600::Adsl2Fast10Mb;
        else if (profile == AdslProfileDisplayName::Adsl2Fast14Mb)
            return AdslProfileDslamName::MA5600::Adsl2Fast14Mb;
        else if (profile == AdslProfileDisplayName::Adsl2Fast18Mb)
            return AdslProfileDslamName::MA5600::Adsl2Fast18Mb;
        else
            return "";
    }
    else if (deviceModel == DeviceModel::MA5300)
    {
        if (profile == AdslProfileDisplayName::AdslFast)
            return AdslProfileDslamName::MA5300::AdslFast;
        else if (profile == AdslProfileDisplayName::AdslInterleave)
            return AdslProfileDslamName::MA5300::AdslInterleave;
        else if (profile == AdslProfileDisplayName::Adsl2Interleave)
            return AdslProfileDslamName::MA5300::Adsl2Interleave;
        else
            return "";
    }
    else if ((deviceModel == DeviceModel::MXA64)
             || (deviceModel == DeviceModel::MXA32))
    {
        if (profile == AdslProfileDisplayName::AdslInterleave)
            return AdslProfileDslamName::MXA::AdslInterleave;
        else if (profile == AdslProfileDisplayName::Adsl2Interleave)
            return AdslProfileDslamName::MXA::Adsl2Interleave;
        else if (profile == AdslProfileDisplayName::AdslGLite)
            return AdslProfileDslamName::MXA::AdslGLite;
        else
            return "";
    }
    else
    {
        return "";
    }
}

QString ToQString(u_char* string, int str_len)
{
    return QString::fromLatin1(reinterpret_cast<char*>(string), str_len);
}
