#ifndef MACINFO_H
#define MACINFO_H

#include <QtCore/QString>
#include <memory>

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

    typedef std::shared_ptr<MacInfo> Ptr;
protected:
    int mNumberPort;
    QString mMac;
    QString mVlanName;
};



#endif // MACINFO_H
