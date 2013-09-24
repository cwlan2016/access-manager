#ifndef XDSLPORT_H
#define XDSLPORT_H

#include <stdafx.h>

#include <customtypes.h>
#include <snmpclient.h>

class XdslPort : public QObject
{
    Q_OBJECT
public:
    XdslPort(int index, long snmpIndex, QObject *parent);

    int index() const;

    long snmpIndex() const;

    int pair() const;
    void setPair(int pair);

    QString name() const;
    void setName(QString name);

    DslPortState::Enum state() const;
    void setState(DslPortState::Enum state);

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

    virtual void fillPrimaryLevelPdu(SnmpClient::Ptr snmpClient, long snmpPortIndex = -1);
    virtual bool parsePrimaryLevelPdu(SnmpClient::Ptr snmpClient);

    virtual void fillSecondaryLevelPdu(SnmpClient::Ptr snmpClient, long snmpPortIndex = -1);
    virtual bool parseSecondaryLevelPdu(SnmpClient::Ptr snmpClient);

    typedef XdslPort *Ptr;


protected:
    int mIndex;         //periodic number in slot
    long mSnmpIndex;    //periodic number in snmp tree

private:
    int mPair;
    QString mName;
    QString mProfile;
    QString mDescription;
    QString mRxAttenuation;
    QString mTxAttenuation;
    QString mTimeLastChange;
    DslPortState::Enum mState;
};

#endif // XDSLPORT_H
