#include "adslport.h"

AdslPort::AdslPort(long index, QObject *parent) :
    XdslPort(index, parent)
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

QString AdslPort::typeLineString(int type)
{
    if (type == 2) {
        return "Fast";
    } else if (type == 3) {
        return "Interleave";
    } else {
        return "другой";
    }
}

QString AdslPort::codingString(int coding)
{
    if (coding == 2) {
        return "dmt";
    } else if (coding == 3) {
        return "cap";
    } else if (coding == 4) {
        return "cam";
    } else {
        return "другой";
    }
}
