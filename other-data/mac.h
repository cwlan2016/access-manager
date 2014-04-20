#ifndef MAC_H
#define MAC_H

#include <stdafx.h>

class Mac : public QObject
{
    Q_OBJECT
public:
    explicit Mac(QObject *parent);

    int port() const;
    void setPort(int port);

    QString mac() const;
    void setMac(QString mac);

    int vlan() const;
    void setVlan(int vlanTag);

    typedef Mac *Ptr;

private:
    int mPort;
    QString mMac;
    int mVlan;
};



#endif // MAC_H
