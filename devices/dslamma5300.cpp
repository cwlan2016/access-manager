#include "dslamma5300.h"

#include <constant.h>
#include <customsnmpfunctions.h>
#include <ports/adslportma5300.h>
#include <ports/shdslportma5300.h>

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
    int koef = 0; //correction depends on the type of board

    if ((boardType == BoardType::AnnexA)
            || (boardType == BoardType::AnnexB)) {
        koef = 201326592;
    } else if (boardType == BoardType::Shdsl) {
        koef = 1476395008;
    }

    return  koef + 65536 * slot + 64 * port;
}

XdslPort::Ptr DslamMa5300::createPort(BoardType::Enum boardType, int boardIndex,
                                      int portIndex, QObject *parent)
{
    long snmpIndex = snmpPortIndex(boardType, boardIndex, portIndex);

    switch (boardType) {
    case BoardType::AnnexA:
    case BoardType::AnnexB:
        return new AdslPortMa5300(portIndex, snmpIndex, parent);
    case BoardType::Shdsl:
        return new ShdslPortMa5300(portIndex, snmpIndex, parent);
    default:
        return new XdslPort(portIndex, snmpIndex, parent);
    }
}

int DslamMa5300::maxLengthPortDescription()
{
    return 64;
}

bool DslamMa5300::setPortDescription(long snmpPortIndex, QString description)
{
    OidPair oid = createOidPair(Mib::ifAlias, 11, snmpPortIndex);

    return snmpSet(ip(), oid, 's', description, mError);
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

    list->push_back(QPair<QString, QString>("Shdsl(Annex B), 2048", "LINE-PROFILE-1"));
    list->push_back(QPair<QString, QString>("Shdsl(Annex B), 4096", "LINE-PROFILE-32"));

    return list;
}
