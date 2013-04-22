#ifndef CONVERTERS_H
#define CONVERTERS_H

#include "customtypes.h"

BoardType::Enum boardTypeFromBoardName(QString boardName);
bool        validIpAddress(QString ip);
QString     typeLineString(int type);
QString     codingString(int coding);
QString     profileExtName(DeviceModel::Enum deviceModel, QString profile);
QString     displayNameProfileToDslamName(DeviceModel::Enum deviceModel,
                                          QString profile);
QString     toString(u_char *string, int str_len);
#endif // CONVERTERS_H
