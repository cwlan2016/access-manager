#ifndef SWITCHPORTINFO_H
#define SWITCHPORTINFO_H

#ifdef _MSC_VER
#include "../snmpclient.h"
#include "../stdafx.h"
#else
#include "snmpclient.h"
#include "stdafx.h"
#endif

class SwitchPortInfo : public QObject
{
    Q_OBJECT
public:
    SwitchPortInfo(QObject *parent);

    int index() const;
    void setIndex(int index);

    QString state() const;
    void setState(QString state);

    QString speedDuplex() const;
    void setSpeedDuplex(QString speedDuplex);

    QString description() const;
    void setDescription(QString description);

    virtual void fillPdu(SnmpClient::Ptr snmpClient, int portIndex = -1);
    virtual void parsePdu(SnmpClient::Ptr snmpClient);

    typedef SwitchPortInfo *Ptr;

protected:
    int mIndex;
    QString mState;
    QString mDescription;
    QString mSpeedDuplex;
};

#endif // SWITCHPORTINFO_H
