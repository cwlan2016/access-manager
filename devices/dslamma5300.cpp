#include "dslamma5300.h"

#include <constant.h>
#include <customsnmpfunctions.h>
#include <ports/adslportma5300.h>

DslamMa5300::DslamMa5300(QObject *parent) :
    Dslam(parent)
{
}

DslamMa5300::DslamMa5300(QString name, QString ip, QObject *parent) :
    Dslam(name, ip, parent)
{
}

DslamMa5300::DslamMa5300(Device *source, QObject *parent) :
    Dslam(source, parent)
{
}

DeviceModel::Enum DslamMa5300::deviceModel() const
{
    return DeviceModel::MA5300;
}

int DslamMa5300::countPorts(BoardType::Enum boardType)
{
    if ((boardType == BoardType::AnnexA)
            || (boardType == BoardType::AnnexB)) {
        return 48;
    } else if (boardType == BoardType::Shdsl) {
        return 24;
    } else {
        return 0;
    }
}

long DslamMa5300::snmpPortIndex(BoardType::Enum boardType, int slot,
                                         int port)
{
    if ((boardType == BoardType::AnnexA)
            || (boardType == BoardType::AnnexB)) {
        return 201326592 + 65536 * slot + 64 * port;
    } else {
        return 0;
    }
}

XdslPort::Ptr DslamMa5300::createPort(BoardType::Enum boardType, int boardIndex,
                                      int portIndex, QObject *parent)
{
    int snmpIndex = snmpPortIndex(boardType, boardIndex, portIndex);

    switch (boardType) {
    case BoardType::AnnexA:
    case BoardType::AnnexB:
        return new AdslPortMa5300(snmpIndex, parent);
    default:
        return new XdslPort(snmpIndex, parent);
    }
}

QList<DslProfile> *DslamMa5300::defaultAdslProfiles()
{
    QList<DslProfile> *list = new QList<DslProfile>();

    list->push_back(QPair<QString, QString>("ADSL, Fast", "LINE-PROFILE-1"));
    list->push_back(QPair<QString, QString>("ADSL, Interleave", "LINE-PROFILE-2"));
    list->push_back(QPair<QString, QString>("ADSL2+, Interleave", "LINE-PROFILE-100"));

    return list;
}

QList<DslProfile> *DslamMa5300::defaultShdslProfiles()
{
    QList<DslProfile> *list = new QList<DslProfile>();
    //TODO: add default shdsl profiles
    return list;
}
