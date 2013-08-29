#include "dslamma5600.h"

#include <constant.h>
#include <customsnmpfunctions.h>
#include <ports/adslportma5600.h>
#include <ports/shdslportma5600.h>

DslamMa5600::DslamMa5600(QObject *parent) :
    Dslam(parent)
{
}

DslamMa5600::DslamMa5600(QString name, QString ip, QObject *parent) :
    Dslam(name, ip, parent)
{
}

DslamMa5600::DslamMa5600(Device *source, QObject *parent) :
    Dslam(source, parent)
{
}

DeviceModel::Enum DslamMa5600::deviceModel() const
{
    return DeviceModel::MA5600;
}

int DslamMa5600::countPorts(BoardType::Enum boardType)
{
    if ((boardType == BoardType::AnnexA)
            || (boardType == BoardType::AnnexB)) {
        return 64;
    } else if (boardType == BoardType::Shdsl) {
        return 32;
    } else {
        return 0;
    }
}

long DslamMa5600::snmpPortIndex(BoardType::Enum boardType, int slot,
                                         int port)
{
    int koef = 0; //correction depends on the type of board

    if ((boardType == BoardType::AnnexA)
            || (boardType == BoardType::AnnexB)) {
        koef = 201326592;
    } else if (boardType == BoardType::Shdsl) {
        koef = 1476395008;
    }

    return koef + 8192 * slot + 64 * port;
}

XdslPort::Ptr DslamMa5600::createPort(BoardType::Enum boardType, int boardIndex,
                                      int portIndex, QObject *parent)
{
    long snmpIndex = snmpPortIndex(boardType, boardIndex, portIndex);

    switch (boardType) {
    case BoardType::AnnexA:
    case BoardType::AnnexB:
        return new AdslPortMa5600(snmpIndex, parent);
    case BoardType::Shdsl:
        return new ShdslPortMa5600(snmpIndex, parent);
    default:
        return new XdslPort(snmpIndex, parent);
    }
}

int DslamMa5600::maxLengthPortDescription()
{
    return 33;
}

bool DslamMa5600::setPortDescription(long snmpPortIndex, QString description)
{
    OidPair oid = createOidPair(Mib::ifAlias, 11, snmpPortIndex);

    return snmpSet(ip(), oid, 's', description, mError);
}

QList<DslProfile> *DslamMa5600::defaultAdslProfiles()
{
    QList<DslProfile> *list = new QList<DslProfile>();

    list->push_back(QPair<QString, QString>("ADSL, Interleave (Default)", "DEFVAL"));
    list->push_back(QPair<QString, QString>("ADSL, Fast", "ADSL LINE PROFILE 1000"));
    list->push_back(QPair<QString, QString>("ADSL, Interleave", "ADSL LINE PROFILE 1001"));
    list->push_back(QPair<QString, QString>("ADSL2+, Interleave", "ADSL LINE PROFILE 1002"));

    return list;
}

QList<DslProfile> *DslamMa5600::defaultShdslProfiles()
{
    QList<DslProfile> *list = new QList<DslProfile>();

    list->push_back(QPair<QString, QString>("Shdsl(Annex A&B), 2048", "DEFVAL"));
    list->push_back(QPair<QString, QString>("Shdsl(Annex A&B), 4096", "SHDSL LINE PROFILE 1"));

    return list;
}


