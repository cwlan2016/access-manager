#ifndef XDSLPORTINFO_H
#define XDSLPORTINFO_H

#include <QtCore/QObject>
#include <QtCore/QString>

class XdslPortInfo : public QObject
{
    Q_OBJECT
public:
    XdslPortInfo(QObject *parent);
    int pair() const;
    QString name() const;
    QString state() const;
    QString desc() const;
    QString profile() const;
    QString timeLastChange() const;
    QString rxAttenuation() const;
    QString txAttenuation() const;
    void setPair(int pair);
    void setName(QString name);
    void setState(QString state);
    void setDesc(QString desc);
    void setProfile(QString profile);
    void setTimeLastChange(QString timeLastChange);
    void setRxAttenuation(QString rxAttenuation);
    void setTxAttenuation(QString txAttenuation);

    typedef XdslPortInfo *Ptr;
protected:
    int mPair;
    QString mName;
    QString mState;
    QString mDesc;
    QString mProfile;
    QString mTimeLastChange;
    QString mRxAttenuation;
    QString mTxAttenuation;
};

#endif // PORTINFO_H
