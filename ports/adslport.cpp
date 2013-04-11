#include "adslportinfo.h"

AdslPortInfo::AdslPortInfo(QObject *parent) :
    XdslPortInfo(parent)
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

void AdslPortInfo::setRxRate(int rxRate)
{
    mRxRate = rxRate;
}

int AdslPortInfo::txRate() const
{
    return mTxRate;
}

void AdslPortInfo::setTxRate(int txRate)
{
    mTxRate = txRate;
}

QString AdslPortInfo::coding() const
{
    return mCoding;
}

void AdslPortInfo::setCoding(QString coding)
{
    mCoding = coding;
}

QString AdslPortInfo::lineType() const
{
    return mLineType;
}

void AdslPortInfo::setLineType(QString lineType)
{
    mLineType = lineType;
}
