#include "snmpconfig.h"

QString SnmpConfig::readCommunity()
{
    return mReadCommunity;
}

void SnmpConfig::setReadCommunity(QString readCommunity)
{
    mReadCommunity = readCommunity;
}

QString SnmpConfig::writeCommunity()
{
    return mWriteCommunity;
}

void SnmpConfig::setWriteCommunity(QString writeCommunity)
{
    mWriteCommunity = writeCommunity;
}

int SnmpConfig::timeout()
{
    return mTimeout;
}

void SnmpConfig::setTimeout(int timeout)
{
    mTimeout = timeout;
}

int SnmpConfig::saveConfigTimeout()
{
    return mSaveConfigTimeout;
}

void SnmpConfig::setSaveConfigTimeout(int timeout)
{
    mSaveConfigTimeout = timeout;
}

int SnmpConfig::port()
{
    return mPort;
}

void SnmpConfig::setPort(int port)
{
    mPort = port;
}

int SnmpConfig::retries()
{
    return mRetries;
}

void SnmpConfig::setRetries(int retries)
{
    mRetries = retries;
}

void SnmpConfig::clear()
{
    mReadCommunity = "";
    mWriteCommunity = "";
    mTimeout = 0;
    mSaveConfigTimeout = 0;
    mPort = 161;
}

void SnmpConfig::toDefault()
{
    mReadCommunity = "public";
    mWriteCommunity = "private";
    mTimeout = 600;
    mSaveConfigTimeout = 10000;
    mPort = 161;
    mRetries = 3;
}

QString SnmpConfig::mReadCommunity = "";
QString SnmpConfig::mWriteCommunity = "";
int SnmpConfig::mTimeout = 0;
int SnmpConfig::mSaveConfigTimeout = 0;
int SnmpConfig::mPort = 0 ;
int SnmpConfig::mRetries = 0;
