#ifndef MACINFO_H
#define MACINFO_H

#ifdef _MSC_VER
#include "../stdafx.h"
#else
#include "stdafx.h"
#endif

class Mac : public QObject
{
    Q_OBJECT
public:
    Mac(QObject *parent);

    int port() const;
    void setPort(int port);

    QString mac() const;
    void setMac(QString mac);

    QString vlanName() const;
    void setVlanName(QString vlanName);

    typedef Mac *Ptr;

private:
    int mPort;
    QString mMac;
    QString mVlanName;
};



#endif // MACINFO_H
