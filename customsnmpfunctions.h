#ifndef CUSTOMSNMPFUNCTIONS_H
#define CUSTOMSNMPFUNCTIONS_H

#include "stdafx.h"

inline bool IsValidSnmpValue(netsnmp_variable_list* vars)
{
    return (vars->type != SNMP_ENDOFMIBVIEW)
            && (vars->type != SNMP_NOSUCHINSTANCE)
            && (vars->type != SNMP_NOSUCHOBJECT);
}

inline oid *CreateOid(const oid *constOid, int oidLen, long *indexs,
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

//{
//    oid *newOid = new oid[oidLen + indexCount];
//    memcpy(newOid, constOid, oidLen * sizeof(oid));
//    for (int i = 0; i < indexCount; i++)
//    newOid[oidLen-1-backOffset + i] = indexs[ i];

//    return newOid;
//}

inline oid *CreateOid(const oid constOid[], int oidLen, long index)
{
    oid *newOid = new oid[oidLen];
    memcpy(newOid, constOid, (oidLen - 1) * sizeof(oid));
    newOid[oidLen - 1]= index;

    return newOid;
}

//inline oid* CreateOid(const oid constOid[], int oidLen, long index, int backOffset = 0)
//{

//    oid *newOid = new oid[oidLen];
//    memcpy(newOid, constOid, oidLen * sizeof(oid));
//    newOid[oidLen-1]= index;
////    newOid[oidLen]= backOffset;

//    qDebug() << "INFO1 = " << newOid;
//        QString s;
//         for(int i = 0; i < oidLen; i++)
//            s += QString::number(newOid[i]) % ".";
//    qDebug() << "INFO2 = " << s;

//    return newOid;
//}

inline oid *CreateOid(const oid constOid[], int oidLen)
{
    oid *newOid = new oid[oidLen];
    memcpy(newOid, constOid, oidLen * sizeof(oid));

    return newOid;
}

#endif // CUSTOMSNMPFUNCTIONS_H
