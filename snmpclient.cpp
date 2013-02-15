#include "snmpclient.h"

#include "Info/snmpconfiginfo.h"

SnmpClient::SnmpClient()
{
    mPdu = 0;
    mSnmpSession = 0;
    mResponsePdu = 0;
}

SnmpClient::~SnmpClient()
{
    if (mResponsePdu)
        snmp_free_pdu(mResponsePdu);

    if (mSnmpSession)
        snmp_close(mSnmpSession);
}

void SnmpClient::setIp(QString ip)
{
    mIp = ip;
}

void SnmpClient::setTimeoutSaveConfig()
{
    mBaseSession.timeout = (long)(SnmpConfigInfo::saveConfigTimeout() * 1000L);
}

bool SnmpClient::setupSession(SessionType::Enum sessionType)
{
    snmp_sess_init(&mBaseSession);

    mBaseSession.version = SNMP_VERSION_2c;

    if (sessionType == SessionType::ReadSession) {
        mBaseSession.community = new uchar[SnmpConfigInfo::readCommunity().length() + 1];
        qstrcpy(reinterpret_cast<char *>(mBaseSession.community),
                SnmpConfigInfo::readCommunity().toLatin1().data());
        mBaseSession.community_len = SnmpConfigInfo::readCommunity().length();
    } else {
        mBaseSession.community = new uchar[SnmpConfigInfo::writeCommunity().length() + 1];
        qstrcpy(reinterpret_cast<char *>(mBaseSession.community),
                SnmpConfigInfo::writeCommunity().toLatin1().data());
        mBaseSession.community_len = SnmpConfigInfo::writeCommunity().length();
    }

    mBaseSession.timeout = (long)(SnmpConfigInfo::timeout() * 1000L);
    mBaseSession.retries = SnmpConfigInfo::retries();
    mBaseSession.remote_port = SnmpConfigInfo::port();
    mBaseSession.peername = new char[mIp.length() + 1];
    memcpy(mBaseSession.peername, mIp.toLatin1().data(), mIp.length());
    mBaseSession.peername[mIp.length()] = '\0';

    return true;
}

bool SnmpClient::openSession()
{
    if (mSnmpSession)
        snmp_close(mSnmpSession);

    mSnmpSession = snmp_open(&mBaseSession);

    return mSnmpSession ? true : false;
}

// Types    SNMP_MSG_GET
//          SNMP_MSG_SET
//          SNMP_MSG_GETNEXT
//          SNMP_MSG_GETBULK
void SnmpClient::createPdu(int pduType, int maxRepetitions)
{
    mPdu = snmp_pdu_create(pduType);
    mPdu->non_repeaters = 0;
    mPdu->max_repetitions = maxRepetitions;
}

// Types    SNMP_MSG_GET
//          SNMP_MSG_SET
//          SNMP_MSG_GETNEXT
//          SNMP_MSG_GETBULK
void SnmpClient::createPduFromResponse(int pduType)
{
    mPdu = snmp_clone_pdu(mResponsePdu);
    mPdu->command = pduType;

    clearResponse();
}

void SnmpClient::clearResponse()
{
    if (mResponsePdu)
        snmp_free_pdu(mResponsePdu);

    mResponsePdu = 0;
}

void SnmpClient::addOid(const OidPair &oid)
{
    snmp_add_null_var(mPdu, oid.first, oid.second);
}

void SnmpClient::addOid(const OidPair &oid, QString value, char type)
{
    snmp_add_var(mPdu, oid.first, oid.second, type, value.toLatin1().data());
}

void SnmpClient::addOid(const oid *someOid, size_t size)
{
    snmp_add_null_var(mPdu, someOid, size);
}

// type next values
// i: INTEGER
// u: unsigned INTEGER
// t: TIMETICKS
// a: IPADDRESS
// o: OBJID
// s: STRING,
// x: HEX STRING
// d: DECIMAL STRING
// b: BITS
void SnmpClient::addOid(const oid *someOid, size_t size, QString value, char type)
{
    snmp_add_var(mPdu, someOid, size, type, value.toLatin1().data());
}

bool SnmpClient::sendRequest()
{
    int status = snmp_synch_response(mSnmpSession, mPdu, &mResponsePdu);

    return (status == STAT_SUCCESS) && (mResponsePdu->errstat == SNMP_ERR_NOERROR);
}

netsnmp_variable_list *SnmpClient::varList()
{
    return mResponsePdu->variables;
}
