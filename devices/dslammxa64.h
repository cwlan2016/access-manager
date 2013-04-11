#ifndef DSLAMMXA64_H
#define DSLAMMXA64_H

#include "dslam.h"

class DslamMxa64 : public Dslam
{
    Q_OBJECT
public:
    explicit DslamMxa64(QObject *parent = 0);
    DslamMxa64(QString name, QString ip, QObject *parent = 0);
    DslamMxa64(Device *source, QObject *parent = 0);

    DeviceModel::Enum deviceModel() const;
    int countPorts(BoardType::Enum boardType);
    long snmpInterfaceIndex(BoardType::Enum boardType, int slot, int port);
};

#endif // DSLAMMXA64_H
