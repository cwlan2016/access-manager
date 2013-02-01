#ifndef ADSLPORTINFO_H
#define ADSLPORTINFO_H

#include "xdslportinfo.h"

class AdslPortInfo : public XdslPortInfo
{
    Q_OBJECT
public:
    AdslPortInfo(QObject *parent);
    int rxRate() const;
    int txRate() const;
    QString coding() const;
    QString lineType() const;
    void setRxRate(int rxRate);
    void setTxRate(int txRate);
    void setCoding(QString coding);
    void setLineType(QString lineType);

    typedef AdslPortInfo *Ptr;
private:
    int mRxRate;
    int mTxRate;
    QString mCoding;
    QString mLineType;
};

#endif // ADSLPORTINFO_H
