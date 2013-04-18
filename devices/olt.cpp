#include "olt.h"

#ifdef _MSC_VER
#include "../constant.h"
#include "../converters.h"
#include "../customsnmpfunctions.h"
#include "../snmpclient.h"
#else
#include "constant.h"
#include "converters.h"
#include "customsnmpfunctions.h"
#include "snmpclient.h"
#endif

Olt::Olt(QObject *parent) :
    Device(parent)
{
}

Olt::Olt(QString name, QString ip, QObject *parent) :
    Device(name, ip, parent)
{
}

Olt::Olt(Device *source, QObject *parent) :
    Device(source, parent)
{
}

QString Olt::serviceProfile(int index)
{
    if (mServiceProfileList.find(index) == mServiceProfileList.end())
        return "";

    return mServiceProfileList[index];
}

void Olt::addServiceProfile(int index, QString profileName)
{
    mServiceProfileList.insert(index, profileName);
}

QString Olt::multicastProfile(int index)
{
    if (mMulticastProfileList.find(index) == mMulticastProfileList.end())
        return "";

    return mMulticastProfileList[index];
}

void Olt::addMulticastProfile(int index, QString profileName)
{
    mMulticastProfileList.insert(index, profileName);
}

OltProfileMap &Olt::serviceProfileList()
{
    return mServiceProfileList;
}

QStringListModel *Olt::serviceProfileListModel(QObject *parent)
{
    return createStringListModel(mServiceProfileList, parent);
}

OltProfileMap &Olt::multicastProfileList()
{
    return mMulticastProfileList;
}

QStringListModel *Olt::multicastProfileListModel(QObject *parent)
{
    return createStringListModel(mMulticastProfileList, parent);
}

DeviceType::Enum Olt::deviceType() const
{
    return DeviceType::Olt;
}

bool Olt::getServiceDataFromDevice()
{
    mError.clear();
    bool result = false;

    OidPair serviceProfileOid;
    OidPair multicastProfileOid;
    if (deviceModel() == DeviceModel::LTP8X) {
        serviceProfileOid = createOidPair(Mib::ltp8xONTServicesName, 13);
        multicastProfileOid = createOidPair(Mib::ltp8xONTMulticastName, 13);
    } else if (deviceModel() == DeviceModel::LTE8ST) {
        serviceProfileOid = createOidPair(Mib::lte8stProfilesRulesDescription, 13);
        multicastProfileOid = createOidPair(Mib::lte8stProfilesIpMulticastDescription, 13);
    } else {
        return false;
    }

    result = getProfileList(mServiceProfileList, serviceProfileOid);
    result |= getProfileList(mMulticastProfileList, multicastProfileOid);

    return result;
}

bool Olt::getProfileList(OltProfileMap &profileList,
                             OidPair oid)
{
    QScopedPointer<SnmpClient> snmp(new SnmpClient());

    snmp->setIp(mIp);

    if (!snmp->setupSession(SessionType::ReadSession)) {
        mError += SnmpErrorStrings::SetupSession % "\n";
        return false;
    }

    if (!snmp->openSession()) {
        mError = SnmpErrorStrings::OpenSession % "\n";
        return false;
    }

    profileList.clear();

    snmp->createPdu(SNMP_MSG_GETNEXT);
    snmp->addOid(oid);

    while (true) {
        if (!snmp->sendRequest()) {
            mError += SnmpErrorStrings::GetInfo % "\n";
            return false;
        }

        netsnmp_variable_list *vars = snmp->varList();

        if (snmp_oid_ncompare(oid.first, oid.second, vars->name,
                              vars->name_length, oid.second) != 0)
            break;

        QString profileName = toString(vars->val.string, vars->val_len);
        int profileIndex = vars->name[vars->name_length - 1];

        profileList.insert(profileIndex, profileName);

        snmp->createPduFromResponse(SNMP_MSG_GETNEXT);
    }

    return true;
}

QStringListModel *Olt::createStringListModel(OltProfileMap &profileList,
                                                        QObject *parent)
{
    QStringList stringList;

    auto it = profileList.constBegin();
    auto end = profileList.constEnd();
    for (; it != end; ++it) {
        QString item = QString("%1. %2")
                .arg(it.key())
                .arg(it.value());
        stringList.push_back(item);
    }

    return new QStringListModel(stringList, parent);
}
