#ifndef SWITCHPORT_H
#define SWITCHPORT_H

#include <stdafx.h>

#include <snmpclient.h>

class SwitchPort : public QObject
{
    Q_OBJECT
public:
    explicit SwitchPort(QObject *parent);

    int index() const;
    void setIndex(int index);

    SwitchPortState::Enum state() const;
    void setState(SwitchPortState::Enum state);

    QString speedDuplex() const;
    void setSpeedDuplex(QString speedDuplex);

    QString description() const;
    void setDescription(QString description);

    virtual void fillPdu(SnmpClient::Ptr snmpClient, int portIndex = -1);
    virtual bool parsePdu(SnmpClient::Ptr snmpClient);

    typedef SwitchPort *Ptr;

protected:
    virtual QString speedDuplexString(long snmpValue);

    int mIndex;

private:
    QString mDescription;
    QString mSpeedDuplex;
    SwitchPortState::Enum mState;
};

#endif // SWITCHPORT_H
