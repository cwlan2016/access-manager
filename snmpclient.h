#ifndef SNMPCLIENT_H
#define SNMPCLIENT_H

#include "stdafx.h"
#include "Info/snmpconfiginfo.h"
#include "converters.h"

class SnmpClient
{
public:
    SnmpClient();
    ~SnmpClient();
    void setIP(QString ip);
    void createPdu(int pduType, int max_repetitions = 1);
    void clearResponsePdu();
    void addOid(QString oid_string);
    void addOid(const oid* _oid, size_t size);
    void addOid(const oid* _oid, size_t size, QString value, char type);
    //void addOid(QString oidString, QString value, char type);
    netsnmp_variable_list* varList();
    bool sendRequest();
    bool setupSession(SessionType sessionType);
    bool openSession();
    void closeSession();
    void setTimeoutSaveConfig();
private:
    netsnmp_session *mSnmpSession;
    netsnmp_session  mBaseSession;
    netsnmp_pdu *mPdu;
    netsnmp_pdu *mResponsePdu;
    QString mIp;
};

#endif // SNMPCLIENT_H
