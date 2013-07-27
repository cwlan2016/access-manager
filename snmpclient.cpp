#include "snmpclient.h"

#include "converters.h"
#include "customsnmpfunctions.h"
#include "configs/snmpconfig.h"

SnmpClient::SnmpClient()
{
    mPdu = 0;
    mSnmpSession = 0;
    mResponsePdu = 0;
}

SnmpClient::~SnmpClient()
{
    freeOid(mGarbageCollector);

    if (mBaseSession.community)
        delete[] mBaseSession.community;

    if (mBaseSession.peername)
        delete[] mBaseSession.peername;

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
    mBaseSession.timeout = (long)(SnmpConfig::saveConfigTimeout() * 1000L);
}

bool SnmpClient::setupSession(SessionType::Enum sessionType)
{
    snmp_sess_init(&mBaseSession);

    mBaseSession.version = SNMP_VERSION_2c;

    if (sessionType == SessionType::ReadSession) {
        mBaseSession.community = new uchar[SnmpConfig::readCommunity().length() + 1];
        qstrcpy(reinterpret_cast<char *>(mBaseSession.community),
                SnmpConfig::readCommunity().toLatin1().data());
        mBaseSession.community_len = SnmpConfig::readCommunity().length();
    } else {
        mBaseSession.community = new uchar[SnmpConfig::writeCommunity().length() + 1];
        qstrcpy(reinterpret_cast<char *>(mBaseSession.community),
                SnmpConfig::writeCommunity().toLatin1().data());
        mBaseSession.community_len = SnmpConfig::writeCommunity().length();
    }

    mBaseSession.timeout = (long)(SnmpConfig::timeout() * 1000L);
    mBaseSession.retries = SnmpConfig::retries();
    mBaseSession.remote_port = SnmpConfig::port();
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
    mGarbageCollector.push_back(oid.first);
    snmp_add_null_var(mPdu, oid.first, oid.second);
}

// type values
// i: INTEGER       u: UNSIGNED INTEGER
// a: IPADDRESS     t: TIMETICKS
// o: OBJID         b: BITS
// s: STRING        d: DECIMAL STRING
// x: HEX STRING
void SnmpClient::addOid(const OidPair &oid, QString value, char type)
{
    mGarbageCollector.push_back(oid.first);
    snmp_add_var(mPdu, oid.first, oid.second, type, value.toLatin1().data());
}

bool SnmpClient::sendRequest()
{
    int status = snmp_synch_response(mSnmpSession, mPdu, &mResponsePdu);

    if (status == STAT_SUCCESS) {
        if (mResponsePdu->errstat != SNMP_ERR_NOERROR) {
            mError = QString::fromUtf8("Ошибка в пакете. Причина: %1.")
                    .arg(snmp_errstring(mResponsePdu->errstat));

            if (mResponsePdu->errindex != 0) {
                int count = 1;
                auto vars = mResponsePdu->variables;
                for (;vars && (count != mResponsePdu->errindex);
                     vars = vars->next_variable, count++)
                                    /*EMPTY*/;
                if (vars) {
                    mError += QString::fromUtf8(" Ошибка в OID: %1.")
                            .arg(toString(vars->name, vars->name_length));
                }
            }

            return false;
        }

        return true;
    } else if (status == STAT_TIMEOUT) {
        mError = QString::fromUtf8("Таймаут. Нет ответа от %1").arg(mSnmpSession->peername);
        return false;
    } else {
        mError = QString::fromUtf8("Неизвестная ошибка");
        return false;
    }
}

QString SnmpClient::error() const
{
    return mError;
}

netsnmp_variable_list *SnmpClient::varList()
{
    return mResponsePdu->variables;
}
