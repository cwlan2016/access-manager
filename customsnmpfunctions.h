#ifndef CUSTOMSNMPFUNCTIONS_H
#define CUSTOMSNMPFUNCTIONS_H

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

inline bool isValidSnmpValue(netsnmp_variable_list *vars)
{
    return (vars->type != SNMP_ENDOFMIBVIEW)
           && (vars->type != SNMP_NOSUCHINSTANCE)
           && (vars->type != SNMP_NOSUCHOBJECT);
}

inline oid *createOid(const oid *constOid, int oidLen, long *indexs,
                      int indexCount = 1, int backOffset = 0)
{
    oid *newOid = new oid[oidLen + indexCount];
    memcpy(newOid, constOid, oidLen * sizeof(oid));

    oidLen += indexCount;
    oidLen -= backOffset + 1;

    for (int i = 0; i < indexCount; ++i)
        newOid[oidLen + i] = indexs[i];

    return newOid;
}

inline oid *createOid(const oid constOid[], int oidLen, long index)
{
    oid *newOid = new oid[oidLen + 1];
    memcpy(newOid, constOid, (oidLen) * sizeof(oid));
    newOid[oidLen] = index;

    return newOid;
}

inline oid *createOid(const oid constOid[], int oidLen)
{
    oid *newOid = new oid[oidLen];
    memcpy(newOid, constOid, oidLen * sizeof(oid));

    return newOid;
}

#endif // CUSTOMSNMPFUNCTIONS_H
