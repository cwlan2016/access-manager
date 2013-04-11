#ifndef SWITCHPORT_H
#define SWITCHPORT_H

#ifdef _MSC_VER
#include "../snmpclient.h"
#include "../stdafx.h"
#else
#include "snmpclient.h"
#include "stdafx.h"
#endif

class SwitchPort : public QObject
{
    Q_OBJECT
public:
    SwitchPort(QObject *parent);

    int index() const;
    void setIndex(int index);

    SwitchPortState::Enum state() const;
    void setState(SwitchPortState::Enum state);

    QString speedDuplex() const;
    void setSpeedDuplex(QString speedDuplex);

    QString description() const;
    void setDescription(QString description);

    virtual void fillPdu(SnmpClient::Ptr snmpClient, int portIndex = -1);
    virtual void parsePdu(SnmpClient::Ptr snmpClient);

    typedef SwitchPort *Ptr;

protected:
    virtual QString speedDuplexString(long snmpValue);

    int mIndex;
    QString mDescription;
    QString mSpeedDuplex;
    SwitchPortState::Enum mState;
};

#endif // SWITCHPORT_H
