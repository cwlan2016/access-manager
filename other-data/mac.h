#ifndef MAC_H
#define MAC_H

#include <stdafx.h>

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



#endif // MAC_H
