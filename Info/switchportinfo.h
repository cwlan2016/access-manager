#ifndef SWITCHPORTINFO_H
#define SWITCHPORTINFO_H

#ifdef _MSC_VER
#include "../stdafx.h"
#else
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

    typedef SwitchPortInfo *Ptr;

protected:
    int mIndex;
    QString mDesc;
    QString mState;
    QString mSpeedDuplex;
};

#endif // SWITCHPORTINFO_H
