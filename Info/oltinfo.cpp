#include "oltinfo.h"

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

OltInfo::OltInfo(QObject *parent) :
    DeviceInfo(parent)
{
}

OltInfo::OltInfo(QString name, QString ip, QObject *parent) :
    DeviceInfo(name, ip, parent)
{
}

QString OltInfo::serviceProfile(int index)
{
    if (mServiceProfileList.find(index) == mServiceProfileList.end())
        return QString();

    return mServiceProfileList[index];
}

void OltInfo::addServiceProfile(int index, QString profileName)
{
    mServiceProfileList.insert(index, profileName);
}

QString OltInfo::multicastProfile(int index)
{
    if (mMulticastProfileList.find(index) == mMulticastProfileList.end())
        return QString();

    return mMulticastProfileList[index];
}

void OltInfo::addMulticastProfile(int index, QString profileName)
{
    mMulticastProfileList.insert(index, profileName);
}

OltProfileMap &OltInfo::serviceProfileList()
{
    return mServiceProfileList;
}

QStringListModel *OltInfo::serviceProfileListModel(QObject *parent)
{
    return createStringListModel(mServiceProfileList, parent);
}

OltProfileMap &OltInfo::multicastProfileList()
{
    return mMulticastProfileList;
}

QStringListModel *OltInfo::multicastProfileListModel(QObject *parent)
{
    return createStringListModel(mMulticastProfileList, parent);
}

DeviceType::Enum OltInfo::deviceType() const
{
    return DeviceType::Olt;
}

bool OltInfo::getServiceDataFromDevice()
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

bool OltInfo::getProfileList(OltProfileMap &profileList,
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

//    int profileNameOidLen = oidLen - 1;
//    oid *profileNameOid = new oid[profileNameOidLen];

//    memcpy(profileNameOid, oidProfileName, profileNameOidLen * sizeof(oid));

//    size_t nextOidLen = oidLen;
//    oid *nextOid = new oid[nextOidLen];

//    memcpy(nextOid, oidProfileName, nextOidLen * sizeof(oid));

    snmp->createPdu(SNMP_MSG_GETNEXT);
    snmp->addOid(oid);
    //snmp->addOid(nextOid, nextOidLen);

    while (true) {
        if (!snmp->sendRequest()) {
            mError += SnmpErrorStrings::GetInfo % "\n";
            return false;
        }

        netsnmp_variable_list *vars = snmp->varList();

        if (snmp_oid_ncompare(oid.first, oid.second, vars->name,
                              vars->name_length, oid.second) != 0)
        //    if (snmp_oid_ncompare(profileNameOid, profileNameOidLen, vars->name,
        //                          vars->name_length, oidLen - 1) != 0)
            break;

        QString profileName = toQString(vars->val.string, vars->val_len);
        int profileIndex = vars->name[vars->name_length - 1];

        profileList.insert(profileIndex, profileName);

        snmp->createPduFromResponse(SNMP_MSG_GETNEXT);
        //delete[] nextOid;

        //nextOid = new oid[vars->name_length];
       // memmove(nextOid, vars->name, vars->name_length * sizeof(oid));
       // nextOidLen = vars->name_length;

        //snmp->clearResponse();
    }

//    snmp->clearResponse();
    //delete[] nextOid;
    //delete[] profileNameOid;

    return true;
}

QStringListModel *OltInfo::createStringListModel(OltProfileMap &profileList,
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
