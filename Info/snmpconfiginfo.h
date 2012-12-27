#ifndef SNMPCONFIGINFO_H
#define SNMPCONFIGINFO_H

#include <QtCore/QString>

class SnmpConfigInfo
{
public:
    static QString readCommunity();
    static QString writeCommunity();
    static int timeout();
    static int saveConfigTimeout();
    static int port();
    static int retries();
    static void setReadCommunity(QString readCommunity);
    static void setWriteCommunity(QString writeCommunity);
    static void setTimeout(int timeout);
    static void setSaveConfigTimeout(int timeout);
    static void setPort(int port);
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
