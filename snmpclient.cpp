#include "snmpclient.h"

SnmpClient::SnmpClient()
{
    mPdu = nullptr;
    mSnmpSession = nullptr;
    mResponsePdu = nullptr;
}

SnmpClient::~SnmpClient()
{
    if(mResponsePdu != nullptr)
        snmp_free_pdu(mResponsePdu);

    if(mSnmpSession != nullptr)
        snmp_close(mSnmpSession);
}

void SnmpClient::setIP(QString ip)
{
    mIp = ip;
}

void SnmpClient::addOid(QString oidString)
{
    uint length = oidString.split(".").count();
    oid* OID = new oid[length];
    size_t  lenOID = length;

    snmp_parse_oid(oidString.toLatin1().data(), OID, &lenOID);
    snmp_add_null_var(mPdu, OID, lenOID);
}

void SnmpClient::addOid(const oid *_oid, size_t size)
{
    snmp_add_null_var(mPdu, _oid, size);
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

//void SnmpClient::addOid(QString oidString, QString value, char type)
//{
//    oid* OID = new oid[oidString.split(".").count()];
//    size_t lenOID = oidString.split(".").count();
//    snmp_parse_oid(oidString.toLatin1().data(), OID, &lenOID);
//    snmp_add_var(mPdu, OID, lenOID, type, value.toLatin1().data());
//}



void SnmpClient::addOid(const oid *_oid, size_t size, QString value, char type)
{
    snmp_add_var(mPdu, _oid, size, type, value.toLatin1().data());
}

netsnmp_variable_list* SnmpClient::varList()
{
    return mResponsePdu->variables;
}

bool SnmpClient::sendRequest()
{
    int status = snmp_synch_response(mSnmpSession, mPdu, &mResponsePdu);

    return (status == STAT_SUCCESS) && (mResponsePdu->errstat == SNMP_ERR_NOERROR);
}

// Types    SNMP_MSG_GET
//          SNMP_MSG_SET
//          SNMP_MSG_GETNEXT
//          SNMP_MSG_GETBULK
void SnmpClient::createPdu(int pduType, int max_repetitions)
{
    mPdu = snmp_pdu_create(pduType);
    mPdu->non_repeaters = 0;
    mPdu->max_repetitions = max_repetitions;
}

bool SnmpClient::setupSession(SessionType sessionType)
{
    snmp_sess_init(&mBaseSession);

    mBaseSession.version = SNMP_VERSION_2c;

    if(sessionType == SessionType::ReadSession)
    {
        mBaseSession.community = new uchar[SnmpConfigInfo::readCommunity().length() + 1];
        qstrcpy(reinterpret_cast<char*>(mBaseSession.community), SnmpConfigInfo::readCommunity().toLatin1().data());
        mBaseSession.community_len = SnmpConfigInfo::readCommunity().length();
    }
    else
    {
        mBaseSession.community = new uchar[SnmpConfigInfo::writeCommunity().length() + 1];
        qstrcpy(reinterpret_cast<char*>(mBaseSession.community), SnmpConfigInfo::writeCommunity().toLatin1().data());
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
    if(mSnmpSession != nullptr)
        closeSession();

    mSnmpSession = snmp_open(&mBaseSession);

    return mSnmpSession ? true : false;
}

void SnmpClient::closeSession()
{
//    библиотека выдает
//    Invalid parameter passed to C runtime function.
//    возможно баг библиотеки net-snmp 4.7.1
//    if (m_snmpSession != NULL)
    snmp_close(mSnmpSession);

    //    m_snmpSession = NULL;
}

void SnmpClient::setTimeoutSaveConfig()
{
    mBaseSession.timeout = (long)(SnmpConfigInfo::saveConfigTimeout() * 1000L);
}

void SnmpClient::clearResponsePdu()
{
    if(mResponsePdu != nullptr)
        snmp_free_pdu(mResponsePdu);

    mResponsePdu = nullptr;
}

