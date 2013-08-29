#ifndef SWITCHDES3526_H
#define SWITCHDES3526_H

#include "switch.h"

class SwitchDes3526 : public Switch
{
    Q_OBJECT
public:
    explicit SwitchDes3526(QObject *parent = 0);
    SwitchDes3526(QString name, QString ip, QObject *parent = 0);
    SwitchDes3526(Device *source, QObject *parent = 0);

    DeviceModel::Enum deviceModel() const;
    int countPorts();
    SwitchPort::Ptr createPort(QObject *parentModel);

    int  maxLengthPortDescription();
    bool setPortDescription(long snmpPortIndex, QString description);
};

#endif // SWITCHDES3526_H
