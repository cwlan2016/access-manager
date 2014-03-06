#ifndef CONVERTERS_H
#define CONVERTERS_H

#include "customtypes.h"

bool        validIpAddress(QString ip);
QString     toString(u_char *string, int str_len);
QString     toString(u_long *oid, int oid_len);
#endif // CONVERTERS_H
