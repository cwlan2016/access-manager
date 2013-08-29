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
    long snmpPortIndex(BoardType::Enum boardType, int slot, int port);
    XdslPort::Ptr createPort(BoardType::Enum boardType, int boardIndex,
                             int portIndex, QObject *parent);

    int  maxLengthPortDescription();
    bool setPortDescription(long snmpPortIndex, QString description);

    static QList<DslProfile> *defaultAdslProfiles();
    static QList<DslProfile> *defaultShdslProfiles();
};

#endif // DSLAMMXA32_H
