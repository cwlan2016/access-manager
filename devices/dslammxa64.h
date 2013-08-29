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
    long snmpPortIndex(BoardType::Enum boardType, int slot, int port);
    XdslPort::Ptr createPort(BoardType::Enum boardType, int boardIndex,
                             int portIndex, QObject *parent);

    int  maxLengthPortDescription();
    bool setPortDescription(long snmpPortIndex, QString description);

    static QList<DslProfile> *defaultAdslProfiles();
    static QList<DslProfile> *defaultShdslProfiles();
};

#endif // DSLAMMXA64_H
