#ifndef XDSLPORTINFO_H
#define XDSLPORTINFO_H

#ifdef _MSC_VER
#include "../stdafx.h"
#else
#include "stdafx.h"
#endif

class XdslPortInfo : public QObject
{
    Q_OBJECT
public:
    XdslPortInfo(QObject *parent);

    int pair() const;
    void setPair(int pair);

    QString name() const;
    void setName(QString name);

    QString state() const;
    void setState(QString state);

    QString profile() const;
    void setProfile(QString profile);

    QString description() const;
    void setDescription(QString description);

    QString rxAttenuation() const;
    void setRxAttenuation(QString rxAttenuation);

    QString txAttenuation() const;
    void setTxAttenuation(QString txAttenuation);

    QString timeLastChange() const;
    void setTimeLastChange(QString timeLastChange);

    typedef XdslPortInfo *Ptr;

protected:
    int mPair;
    QString mName;
    QString mState;
    QString mProfile;
    QString mDescription;
    QString mRxAttenuation;
    QString mTxAttenuation;
    QString mTimeLastChange;
};

#endif // PORTINFO_H
