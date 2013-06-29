#ifndef SNMPCLIENT_H
#define SNMPCLIENT_H

#include "customtypes.h"

class SnmpClient
{
public:
    SnmpClient();
    ~SnmpClient();

    void setIp(QString ip);
    void setTimeoutSaveConfig();
    bool setupSession(SessionType::Enum sessionType);
    bool openSession();

    void createPdu(int pduType, int maxRepetitions = 1);
    void createPduFromResponse(int pduType);
    void clearResponse();

    void addOid(const OidPair &oid);
    void addOid(const OidPair &oid, QString value, char type);

    bool sendRequest();

    QString error() const;

    netsnmp_variable_list *varList();

    typedef SnmpClient *Ptr;

private:
    QString mIp;

    netsnmp_pdu *mPdu;
    netsnmp_pdu *mResponsePdu;

    netsnmp_session *mSnmpSession;
    netsnmp_session mBaseSession;

    QString mError;

    QVector<const oid *> mGarbageCollector;
};

#endif // SNMPCLIENT_H
