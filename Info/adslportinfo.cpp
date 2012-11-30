#include "adslportinfo.h"

AdslPortInfo::AdslPortInfo() :
    XdslPortInfo()
{
    mRxRate = 0;
    mTxRate = 0;
    mCoding = "dmt";
    mLineType = "Fast";
}

int AdslPortInfo::rxRate() const
{
    return mRxRate;
}

int AdslPortInfo::txRate() const
{
    return mTxRate;
}

QString AdslPortInfo::coding() const
{
    return mCoding;
}

QString AdslPortInfo::lineType() const
{
    return mLineType;
}

void AdslPortInfo::setRxRate(int rxRate)
{
    mRxRate = rxRate;
}

void AdslPortInfo::setTxRate(int txRate)
{
    mTxRate = txRate;
}

void AdslPortInfo::setCoding(QString coding)
{
    mCoding = coding;
}

void AdslPortInfo::setLineType(QString lineType)
{
    mLineType = lineType;
}
