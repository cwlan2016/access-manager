#ifndef MACINFO_H
#define MACINFO_H

#ifdef _MSC_VER
#include "../stdafx.h"
#else
#include "stdafx.h"
#endif

class MacInfo : public QObject
{
    Q_OBJECT
public:
    MacInfo(QObject *parent);

    int port() const;
    void setPort(int port);

    QString mac() const;
    void setMac(QString mac);

    QString vlanName() const;
    void setVlanName(QString vlanName);

    typedef MacInfo *Ptr;

private:
    int mPort;
    QString mMac;
    QString mVlanName;
};



#endif // MACINFO_H
