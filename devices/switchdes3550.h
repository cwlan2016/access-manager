#ifndef SWITCHDES3550_H
#define SWITCHDES3550_H

#include "switch.h"

class SwitchDes3550 : public Switch
{
    Q_OBJECT
public:
    explicit SwitchDes3550(QObject *parent = 0);
    SwitchDes3550(QString name, QString ip, QObject *parent = 0);
    explicit SwitchDes3550(Device *source, QObject *parent = 0);

    DeviceModel::Enum deviceModel() const;
    int countPorts();
    int sizePortBitArray();
    SwitchPort::Ptr createPort(QObject *parentModel);

    int  maxLengthPortDescription();
    bool setPortDescription(long snmpPortIndex, QString description);
};

#endif // SWITCHDES3550_H
