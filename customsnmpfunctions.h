#ifndef CUSTOMSNMPFUNCTIONS_H
#define CUSTOMSNMPFUNCTIONS_H

#include "stdafx.h"

#include "converters.h"
#include "customtypes.h"

inline bool isValidSnmpValue(netsnmp_variable_list *vars)
{
    return vars && (vars->type != SNMP_ENDOFMIBVIEW)
           && (vars->type != SNMP_NOSUCHINSTANCE)
           && (vars->type != SNMP_NOSUCHOBJECT);
}

inline OidPair createOidPair(const oid constOid[], int oidLen)
{
    oid *newOid = new oid[oidLen];
    memcpy(newOid, constOid, oidLen * sizeof(oid));

    return qMakePair(newOid, oidLen);
}

inline OidPair createOidPair(const oid constOid[], int oidLen, long index)
{
    oid *newOid = new oid[oidLen + 1];
    memcpy(newOid, constOid, (oidLen) * sizeof(oid));
    newOid[oidLen] = index;

    return qMakePair(newOid, oidLen + 1);
}

inline OidPair createOidPair(const oid *constOid, int oidLen, long *indexs,
                      int indexCount = 1, int backOffset = 0)
{
    int fullLen = oidLen + indexCount;
    oid *newOid = new oid[fullLen];
    memcpy(newOid, constOid, oidLen * sizeof(oid));

    oidLen -= backOffset;

    for (int i = 0; i < indexCount; ++i)
        newOid[oidLen + i] = indexs[i];

    return qMakePair(newOid, fullLen);
}

bool snmpSet(QString ipAddress, OidPair oid, char type, QString value, QString &error);

#endif // CUSTOMSNMPFUNCTIONS_H
