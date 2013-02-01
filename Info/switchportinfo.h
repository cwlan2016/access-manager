#ifndef SWITCHPORTINFO_H
#define SWITCHPORTINFO_H

#include <QtCore/QObject>
#include <QtCore/QString>

class SwitchPortInfo : public QObject
{
    Q_OBJECT
public:
    SwitchPortInfo(QObject *parent);
    int number() const;
    QString state() const;
    QString desc() const;
    QString speedDuplex() const;
    void setNumber(int number);
    void setState(QString state);
    void setDesc(QString desc);
    void setSpeedDuplex(QString speedDuplex);

    typedef SwitchPortInfo *Ptr;
protected:
    int mNumber;
    QString mState;
    QString mDesc;
    QString mSpeedDuplex;
};

#endif // SWITCHPORTINFO_H
