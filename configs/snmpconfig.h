#ifndef SNMPCONFIGINFO_H
#define SNMPCONFIGINFO_H

#ifdef _MSC_VER
#include "../stdafx.h"
#else
#include "stdafx.h"
#endif

class SnmpConfigInfo
{
public:
    static QString readCommunity();
    static void setReadCommunity(QString readCommunity);

    static QString writeCommunity();
    static void setWriteCommunity(QString writeCommunity);

    static int timeout();
    static void setTimeout(int timeout);

    static int saveConfigTimeout();
    static void setSaveConfigTimeout(int timeout);

    static int port();
    static void setPort(int port);

    static int retries();
    static void setRetries(int retries);

    static void clear();
    static void toDefault();

private:
    static QString mWriteCommunity;
    static QString mReadCommunity;
    static int mTimeout;
    static int mSaveConfigTimeout;
    static int mPort;
    static int mRetries;
};

#endif // SNMPCONFIGINFO_H
