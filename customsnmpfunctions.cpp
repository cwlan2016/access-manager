#include "customsnmpfunctions.h"

#include "constant.h"
#include "snmpclient.h"

//type - snmp type: i, s, x and etc.
bool snmpSet(QString ipAddress, OidPair oid, char type, QString value, QString &error)
{
    QScopedPointer<SnmpClient> snmpClient(new SnmpClient());

    snmpClient->setIp(ipAddress);

    if (!snmpClient->setupSession(SessionType::WriteSession)) {
        error = SnmpErrorStrings::SetupSession;
        return false;
    }

    if (!snmpClient->openSession()) {
        error = SnmpErrorStrings::OpenSession;
        return false;
    }

    snmpClient->createPdu(SNMP_MSG_SET);
    snmpClient->addOid(oid, value, type);
    bool result = snmpClient->sendRequest();

    if (!result)
        error = snmpClient->error();

    return result;
}
