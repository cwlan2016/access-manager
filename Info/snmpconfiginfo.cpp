#include "snmpconfiginfo.h"

QString SnmpConfigInfo::readCommunity()
{
    return mReadCommunity;
}

QString SnmpConfigInfo::writeCommunity()
{
    return mWriteCommunity;
}

int SnmpConfigInfo::timeout()
{
    return mTimeout;
}

int SnmpConfigInfo::saveConfigTimeout()
{
    return mSaveConfigTimeout;
}

int SnmpConfigInfo::port()
{
    return mPort;
}

int SnmpConfigInfo::retries()
{
    return mRetries;
}

void SnmpConfigInfo::setReadCommunity(QString readCommunity)
{
    mReadCommunity = readCommunity;
}

void SnmpConfigInfo::setWriteCommunity(QString writeCommunity)
{
    mWriteCommunity = writeCommunity;
}

void SnmpConfigInfo::setTimeout(int timeout)
{
    mTimeout = timeout;
}

void SnmpConfigInfo::setSaveConfigTimeout(int timeout)
{
    mSaveConfigTimeout = timeout;
}

void SnmpConfigInfo::setPort(int port)
{
    mPort = port;
}

void SnmpConfigInfo::setRetries(int retries)
{
    mRetries = retries;
}

void SnmpConfigInfo::clear()
{
    mReadCommunity = "";
    mWriteCommunity = "";
    mTimeout = 0;
    mSaveConfigTimeout = 0;
    mPort = 161;
}

void SnmpConfigInfo::toDefault()
{
    mReadCommunity = "public";
    mWriteCommunity = "private";
    mTimeout = 600;
    mSaveConfigTimeout = 10000;
    mPort = 161;
    mRetries = 3;
}

QString SnmpConfigInfo::mWriteCommunity = "";
QString SnmpConfigInfo::mReadCommunity = "";
int SnmpConfigInfo::mTimeout = 0;
int SnmpConfigInfo::mSaveConfigTimeout = 0;
int SnmpConfigInfo::mPort = 0 ;
int SnmpConfigInfo::mRetries = 0;
