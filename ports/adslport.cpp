#include "adslportinfo.h"

AdslPort::AdslPort(QObject *parent) :
    XdslPort(parent)
{
    mRxRate = 0;
    mTxRate = 0;
    mCoding = "dmt";
    mLineType = "Fast";
}

int AdslPort::rxRate() const
{
    return mRxRate;
}

void AdslPort::setRxRate(int rxRate)
{
    mRxRate = rxRate;
}

int AdslPort::txRate() const
{
    return mTxRate;
}

void AdslPort::setTxRate(int txRate)
{
    mTxRate = txRate;
}

QString AdslPort::coding() const
{
    return mCoding;
}

void AdslPort::setCoding(QString coding)
{
    mCoding = coding;
}

QString AdslPort::lineType() const
{
    return mLineType;
}

void AdslPort::setLineType(QString lineType)
{
    mLineType = lineType;
}
