#ifndef DSLAMMA5600_H
#define DSLAMMA5600_H

#include "dslam.h"

class DslamMa5600 : public Dslam
{
    Q_OBJECT
public:
    explicit DslamMa5600(QObject *parent = 0);
    DslamMa5600(QString name, QString ip, QObject *parent = 0);
    explicit DslamMa5600(Device *source, QObject *parent = 0);

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

#endif // DSLAMMA5600_H
