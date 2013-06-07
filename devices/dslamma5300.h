#ifndef DSLAMMA5300_H
#define DSLAMMA5300_H

#include "dslam.h"

class DslamMa5300 : public Dslam
{
    Q_OBJECT
public:
    explicit DslamMa5300(QObject *parent = 0);
    DslamMa5300(QString name, QString ip, QObject *parent = 0);
    DslamMa5300(Device *source, QObject *parent = 0);

    DeviceModel::Enum deviceModel() const;
    int countPorts(BoardType::Enum boardType);
    long snmpPortIndex(BoardType::Enum boardType, int slot, int port);
    XdslPort::Ptr createPort(BoardType::Enum boardType, int boardIndex,
                             int portIndex, QObject *parent);

    static QList<DslProfile> *defaultAdslProfiles();
    static QList<DslProfile> *defaultShdslProfiles();
};

#endif // DSLAMMA5300_H
