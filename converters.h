#ifndef CONVERTERS_H
#define CONVERTERS_H

#include "customtypes.h"

BoardType::Enum boardTypeFromBoardName(QString boardName);
bool        validIpAddress(QString ip);
QString     toString(u_char *string, int str_len);
QString     oidToString(u_long *oid, int oid_len);
#endif // CONVERTERS_H
