#include "snmpconfiginfo.h"

QString SnmpConfigInfo::readCommunity()
{
    return mReadCommunity;
}

void SnmpConfigInfo::setReadCommunity(QString readCommunity)
{
    mReadCommunity = readCommunity;
}

QString SnmpConfigInfo::writeCommunity()
{
    return mWriteCommunity;
}

void SnmpConfigInfo::setWriteCommunity(QString writeCommunity)
{
    mWriteCommunity = writeCommunity;
}

int SnmpConfigInfo::timeout()
{
    return mTimeout;
}

void SnmpConfigInfo::setTimeout(int timeout)
{
    mTimeout = timeout;
}

int SnmpConfigInfo::saveConfigTimeout()
{
    return mSaveConfigTimeout;
}

void SnmpConfigInfo::setSaveConfigTimeout(int timeout)
{
    mSaveConfigTimeout = timeout;
}

int SnmpConfigInfo::port()
{
    return mPort;
}

void SnmpConfigInfo::setPort(int port)
{
    mPort = port;
}

int SnmpConfigInfo::retries()
{
    return mRetries;
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

QString SnmpConfigInfo::mReadCommunity = QString();
QString SnmpConfigInfo::mWriteCommunity = QString();
int SnmpConfigInfo::mTimeout = 0;
int SnmpConfigInfo::mSaveConfigTimeout = 0;
int SnmpConfigInfo::mPort = 0 ;
int SnmpConfigInfo::mRetries = 0;
