#ifndef CONVERTERS_H
#define CONVERTERS_H

#include "customtypes.h"

bool        validIpAddress(QString ip);
QString     toString(quint8 *cstring, int strLen);
QString     toString(ulong *oid, int oidLen);
#endif // CONVERTERS_H
