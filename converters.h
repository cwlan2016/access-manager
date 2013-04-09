#ifndef CONVERTERS_H
#define CONVERTERS_H

#include "customtypes.h"

BoardType::Enum boardTypeFromBoardName(QString boardName);
QString     decMacAddressToHex(oid *macAddressOid, int length);
bool        validIpAddress(QString ip);
QBitArray   ucharToQBitArray(DeviceModel::Enum deviceModel, uchar *str);
QString     qbitArrayToHexString(DeviceModel::Enum switchType,
                                 const QBitArray &array, bool ismv);
QString     typeLineString(int type);
QString     codingString(int coding);
QString     profileExtName(DeviceModel::Enum deviceModel, QString profile);
QString     displayNameProfileToDslamName(DeviceModel::Enum deviceModel,
                                          QString profile);
QString     toString(u_char *string, int str_len);
#endif // CONVERTERS_H
