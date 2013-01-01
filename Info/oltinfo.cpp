#include "oltinfo.h"

#include <QtCore/QStringBuilder>
#ifdef _MSC_VER
#include "../constant.h"
#include "../snmpclient.h"
#else
#include "constant.h"
#include "snmpclient.h"
#endif

OltInfo::OltInfo() :
    DeviceInfo()
{
}

OltInfo::OltInfo(QString name, QString ip, DeviceModel::Enum deviceModel) :
    DeviceInfo(name, ip, deviceModel)
{
}

QString OltInfo::serviceProfile(int index)
{
    if (mServiceProfileList.find(index) == mServiceProfileList.end())
        return "";

    return mServiceProfileList[index];
}

QString OltInfo::multicastProfile(int index)
{
    if (mMulticastProfileList.find(index) == mMulticastProfileList.end())
        return "";

    return mMulticastProfileList[index];
}

void OltInfo::addServiceProfile(int index, QString profileName)
{
    mServiceProfileList.insert(OltProfileMap::value_type(index, profileName));
}

void OltInfo::addMulticastProfile(int index, QString profileName)
{
    mMulticastProfileList.insert(OltProfileMap::value_type(index, profileName));
}

bool OltInfo::getServiceDataFromDevice()
{
    mError.clear();
    bool result = false;

    if (mDeviceModel == DeviceModel::LTP8X) {
        result = getProfileList(mServiceProfileList, Mib::ltp8xONTServicesName, 14);
        result |= getProfileList(mMulticastProfileList, Mib::ltp8xONTMulticastName, 14);
    } else if (mDeviceModel == DeviceModel::LTE8ST) {
        result = getProfileList(mServiceProfileList, Mib::lte8stProfilesRulesDescription, 14);
        result |= getProfileList(mMulticastProfileList, Mib::lte8stProfilesIpMulticastDescription, 14);
    } else {
        return false;
    }

    return result;
}

OltProfileMap &OltInfo::serviceProfileList()
{
    return mServiceProfileList;
}

OltProfileMap &OltInfo::multicastProfileList()
{
    return mMulticastProfileList;
}

QStringListModel *OltInfo::serviceProfileListModel(QObject *parent)
{
    return createStringListModelFromMap(mServiceProfileList, parent);
}

QStringListModel *OltInfo::multicastProfileListModel(QObject *parent)
{
    return createStringListModelFromMap(mMulticastProfileList, parent);
}

bool OltInfo::getProfileList(OltProfileMap &profileList, const oid *oidProfileName, int oidLen)
{
    profileList.clear();

    std::unique_ptr<SnmpClient> snmp(new SnmpClient());

    snmp->setIP(mIp);

    if (!snmp->setupSession(SessionType::ReadSession)) {
        mError += SnmpErrors::SetupSession % "\n";
        return false;
    }

    if (!snmp->openSession()) {
        mError = SnmpErrors::OpenSession % "\n";
        return false;
    }

    int profileNameOidLen = oidLen - 1;
    oid *profileNameOid = new oid[profileNameOidLen];

    memcpy(profileNameOid, oidProfileName, profileNameOidLen * sizeof(oid));

    size_t nextOidLen = oidLen;
    oid *nextOid = new oid[nextOidLen];

    memcpy(nextOid, oidProfileName, nextOidLen * sizeof(oid));

    while (true) {
        snmp->createPdu(SNMP_MSG_GETNEXT);
        snmp->addOid(nextOid, nextOidLen);

        if (!snmp->sendRequest()) {
            mError += SnmpErrors::GetInfo % "\n";
            return false;
        }

        netsnmp_variable_list *vars = snmp->varList();

        if (snmp_oid_ncompare(profileNameOid, profileNameOidLen, vars->name, vars->name_length, oidLen - 1) != 0)
            break;

        QString profileName = QString::fromLatin1((char *)vars->val.string, vars->val_len);
        int profileIndex = vars->name[vars->name_length - 1];

        profileList.insert(OltProfileMap::value_type(profileIndex, profileName));

        delete[] nextOid;

        nextOid = new oid[vars->name_length];
        memmove(nextOid, vars->name, vars->name_length * sizeof(oid));
        nextOidLen = vars->name_length;

        snmp->clearResponsePdu();
    }

    delete[] nextOid;
    delete[] profileNameOid;

    return true;
}

QStringListModel *OltInfo::createStringListModelFromMap(OltProfileMap &profileList, QObject *parent)
{
    QStringList stringList;

    auto it = profileList.begin();
    auto end = profileList.end();
    for (; it != end; ++it) {
        stringList.push_back(QString::number((*it).first) % ". " % (*it).second);
    }

    return new QStringListModel(stringList, parent);
}
