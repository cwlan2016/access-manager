#include "switchinfo.h"

#ifdef _MSC_VER
#include "../constant.h"
#include "../converters.h"
#include "../customsnmpfunctions.h"
#else
#include "constant.h"
#include "converters.h"
#include "customsnmpfunctions.h"
#endif

SwitchInfo::SwitchInfo(QObject *parent) :
    DeviceInfo(parent),
    mInetVlanTag(0),
    mIptvVlanTag(0)
{
}

SwitchInfo::SwitchInfo(QString name, QString ip, QObject *parent) :
    DeviceInfo(name, ip, parent)
{
}

int SwitchInfo::inetVlanTag() const
{
    return mInetVlanTag;
}

void SwitchInfo::setInetVlanTag(int vlanTag)
{
    mInetVlanTag = vlanTag;
}

int SwitchInfo::iptvVlanTag() const
{
    return mIptvVlanTag;
}

void SwitchInfo::setIptvVlanTag(int vlanTag)
{
    mIptvVlanTag = vlanTag;
}

DeviceType::Enum SwitchInfo::deviceType() const
{
    return DeviceType::Switch;
}

bool SwitchInfo::getServiceDataFromDevice()
{
    mError.clear();

    QScopedPointer<SnmpClient> snmpClient(new SnmpClient());

    snmpClient->setIp(mIp);

    if (!snmpClient->setupSession(SessionType::ReadSession)) {
        mError = SnmpErrorStrings::SetupSession;
        return false;
    }

    if (!snmpClient->openSession()) {
        mError = SnmpErrorStrings::OpenSession;
        return false;
    }

    snmpClient->createPdu(SNMP_MSG_GETNEXT);
    snmpClient->addOidPair(createOidPair(Mib::dot1qVlanStaticName, 13));

    bool findedInet, findedIptv;
    findedInet = false;
    findedIptv = false;

    while (snmpClient->sendRequest()) {
        netsnmp_variable_list *vars = snmpClient->varList();

        if (snmp_oid_ncompare(Mib::dot1qVlanStaticName, 13, vars->name,
                              vars->name_length, 13) != 0)
            break;

        QString vlanName = toQString(vars->val.string, vars->val_len);

        if (vlanName == "Inet") {
            findedInet = true;
            mInetVlanTag = vars->name[13];

            if (findedIptv)
                break;
        } else if (vlanName == "IPTV_Unicast") {
            findedIptv = true;
            mIptvVlanTag = vars->name[13];

            if (findedInet)
                break;
        }

        snmpClient->createPduFromResponse(SNMP_MSG_GETNEXT);
    }

    if (!findedInet && !findedInet)
        mError = QString::fromUtf8("Ошибка: вланы для интернета и iptv на коммутаторе %1 [%2] не найдены.").arg(mName, mIp);
    else if (!findedInet)
        mError = QString::fromUtf8("Ошибка: влан для интернета на коммутаторе %1 [%2] не найден.").arg(mName, mIp);
    else if (!findedIptv)
        mError = QString::fromUtf8("Ошибка: влан для iptv на коммутаторе %1 [%2] не найден.").arg(mName, mIp);

    return findedInet && findedIptv;
}

bool SwitchInfo::saveConfig()
{
    QScopedPointer<SnmpClient> snmp(new SnmpClient());

    snmp->setIp(mIp);

    if ((deviceModel() == DeviceModel::DES3526)
            || (deviceModel() == DeviceModel::DES3550)) {
        if (!snmp->setupSession(SessionType::ReadSession)) {
            mError = SnmpErrorStrings::SetupSession;
            return false;
        }

        if (!snmp->openSession()) {
            mError = SnmpErrorStrings::OpenSession;
            return false;
        }

        snmp->createPdu(SNMP_MSG_GET);

        snmp->addOid(Mib::agentStatusSaveCfg, 12);

        if (snmp->sendRequest()) {
            netsnmp_variable_list *vars = snmp->varList();

            if (isValidSnmpValue(vars)) {
                if (*(vars->val.integer) == 2) {

                    mError = QString::fromUtf8("Ошибка: В данный момент уже идет сохранение конфигурации коммутатора.");
                    return false;
                }
            } else {
                mError = QString::fromUtf8("Ошибка: Запрошенная информация на коммутаторе не найдена.");
                return false;
            }
        } else {
            mError = QString::fromUtf8("Ошибка: Не удалось получить данные о текущем состоянии коммутатора.");
            return false;
        }
    }

    if (!snmp->setupSession(SessionType::WriteSession)) {
        mError = SnmpErrorStrings::SetupSession;
        return false;
    }

    snmp->setTimeoutSaveConfig();

    if (!snmp->openSession()) {
        mError = SnmpErrorStrings::OpenSession;
        return false;
    }

    snmp->createPdu(SNMP_MSG_SET);

    OidPair saveOid = createOidPair(Mib::agentSaveCfg, 12);

    if ((deviceModel() == DeviceModel::DES3526)
            || (deviceModel() == DeviceModel::DES3550)) {
        snmp->addOid(saveOid, "3", 'i');
    } else if (deviceModel() == DeviceModel::DES3528) {
        snmp->addOid(saveOid, "5", 'i');
    } else {
        mError = QString::fromUtf8("Неизвестная модель коммутатора.");
        return false;
    }

    if (!snmp->sendRequest()) {
        mError = QString::fromUtf8("Ошибка: Не удалось отправить запрос на сохранение конфигурации коммутатора.");
        return false;
    }

    return true;
}
