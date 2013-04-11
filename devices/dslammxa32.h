#ifndef DSLAMMXA32_H
#define DSLAMMXA32_H

#include "dslam.h"

class DslamMxa32 : public Dslam
{
    Q_OBJECT
public:
    explicit DslamMxa32(QObject *parent = 0);
    DslamMxa32(QString name, QString ip, QObject *parent = 0);
    DslamMxa32(Device *source, QObject *parent = 0);

    DeviceModel::Enum deviceModel() const;
    int countPorts(BoardType::Enum boardType);
    long snmpInterfaceIndex(BoardType::Enum boardType, int slot, int port);
};

#endif // DSLAMMXA32_H
