#ifndef MACINFO_H
#define MACINFO_H

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

class MacInfo
{
public:
    MacInfo();
    int numberPort() const;
    QString mac() const;
    QString vlanName() const;
    void setNumberPort(int numberPort);
    void setMac(QString mac);
    void setVlanName(QString vlanName);

    typedef QSharedPointer<MacInfo> Ptr;
private:
    QString mMac;
    int mNumberPort;
    QString mVlanName;
};



#endif // MACINFO_H
