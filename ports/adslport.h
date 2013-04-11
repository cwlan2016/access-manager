#ifndef ADSLPORT_H
#define ADSLPORT_H

#include "xdslport.h"

class AdslPort : public XdslPort
{
    Q_OBJECT
public:
    AdslPort(QObject *parent);

    int rxRate() const;
    void setRxRate(int rxRate);

    int txRate() const;
    void setTxRate(int txRate);

    QString coding() const;
    void setCoding(QString coding);

    QString lineType() const;
    void setLineType(QString lineType);

    typedef AdslPort *Ptr;

private:
    int mRxRate;
    int mTxRate;
    QString mCoding;
    QString mLineType;
};

#endif // ADSLPORT_H
