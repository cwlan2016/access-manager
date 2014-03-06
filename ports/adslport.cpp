#include "adslport.h"

AdslPort::AdslPort(int index, long snmpIndex, QObject *parent) :
    XdslPort(index, snmpIndex, parent)
{
    setRxCurrRate(0);
    setTxCurrRate(0);
    setCoding("dmt");
    setLineType("Fast");
}

int AdslPort::rxAttainableRate() const
{
    return mRxAttainableRate;
}

void AdslPort::setRxAttainableRate(int rxAttainableRate)
{
    mRxAttainableRate = rxAttainableRate;
}

int AdslPort::txAttainableRate() const
{
    return mTxAttainableRate;
}

void AdslPort::setTxAttainableRate(int txAttainableRate)
{
    mTxAttainableRate = txAttainableRate;
}

int AdslPort::rxCurrRate() const
{
    return mRxCurrRate;
}

void AdslPort::setRxCurrRate(int rxRate)
{
    mRxCurrRate = rxRate;
}

int AdslPort::txCurrRate() const
{
    return mTxCurrRate;
}

void AdslPort::setTxCurrRate(int txRate)
{
    mTxCurrRate = txRate;
}

int AdslPort::rxPrevRate() const
{
    return mRxPrevRate;
}

void AdslPort::setRxPrevRate(int rxPrevRate)
{
    mRxPrevRate = rxPrevRate;
}

int AdslPort::txPrevRate() const
{
    return mTxPrevRate;
}

void AdslPort::setTxPrevRate(int txPrevRate)
{
    mTxPrevRate = txPrevRate;
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
