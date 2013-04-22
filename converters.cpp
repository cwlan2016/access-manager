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
        return "";
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

    return "";

}

QString toString(u_char *string, int str_len)
{
    return QString::fromLatin1(reinterpret_cast<char *>(string), str_len);
}
