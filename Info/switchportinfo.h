#ifndef SWITCHPORTINFO_H
#define SWITCHPORTINFO_H

#include <QtCore/QSharedPointer>
#include <QtCore/QString>

class SwitchPortInfo
{
public:
    SwitchPortInfo();
    int number() const;
    QString state() const;
    QString desc() const;
    QString speedDuplex() const;
    void setNumber(int number);
    void setState(QString state);
    void setDesc(QString desc);
    void setSpeedDuplex(QString speedDuplex);

    typedef QSharedPointer<SwitchPortInfo> Ptr;
protected:
    int mNumber;
    QString mState;
    QString mDesc;
    QString mSpeedDuplex;
};



#endif // SWITCHPORTINFO_H
