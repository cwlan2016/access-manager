#ifndef ADSLPORT_H
#define ADSLPORT_H

#include "xdslport.h"

class AdslPort : public XdslPort
{
    Q_OBJECT
public:
    AdslPort(long index, QObject *parent);

    int rxAttainableRate() const;
    void setRxAttainableRate(int rxAttainableRate);

    int txAttainableRate() const;
    void setTxAttainableRate(int txAttainableRate);

    int rxCurrRate() const;
    void setRxCurrRate(int rxCurrRate);

    int txCurrRate() const;
    void setTxCurrRate(int txCurrRate);

    int rxPrevRate() const;
    void setRxPrevRate(int rxPrevRate);

    int txPrevRate() const;
    void setTxPrevRate(int txPrevRate);

    QString coding() const;
    void setCoding(QString coding);

    QString lineType() const;
    void setLineType(QString lineType);

    typedef AdslPort *Ptr;

protected:
    QString typeLineString(int type);
    QString codingString(int coding);

    int mRxAttainableRate;
    int mTxAttainableRate;
    int mRxCurrRate;
    int mTxCurrRate;
    int mRxPrevRate;
    int mTxPrevRate;

    QString mCoding;
    QString mLineType;
};

#endif // ADSLPORT_H
