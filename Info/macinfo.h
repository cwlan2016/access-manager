#ifndef MACINFO_H
#define MACINFO_H

#include <QtCore/QObject>
#include <QtCore/QString>

class MacInfo : public QObject
{
    Q_OBJECT
public:
    MacInfo(QObject *parent);
    int numberPort() const;
    QString mac() const;
    QString vlanName() const;
    void setNumberPort(int numberPort);
    void setMac(QString mac);
    void setVlanName(QString vlanName);

    typedef MacInfo *Ptr;
private:
    QString mMac;
    int mNumberPort;
    QString mVlanName;
};



#endif // MACINFO_H
